package com.physics.sim;

import com.physics.sim.nativebridge.NativePhysicsWorld;
import com.physics.sim.ui.Sidebar;
import com.physics.sim.ui.StatsPanel;
import javafx.application.Application;
import javafx.animation.AnimationTimer;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.control.Label;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.StackPane;
import javafx.scene.paint.Color;
import javafx.stage.Stage;

import java.util.ArrayList;
import java.util.List;

/**
 * Physics Simulator Pro — Version 2.0
 *
 * Layout:
 *   TOP    → header bar
 *   LEFT   → Sidebar
 *   CENTER → Simulation canvas
 *   BOTTOM → Live statistics panel
 */
public class MainApp extends Application {

    private static final double SCALE = 15.0;

    private final NativePhysicsWorld nativeWorld = new NativePhysicsWorld();
    private final Canvas canvas = new Canvas(900, 560);

    private AnimationTimer timer;
    private boolean isRunning = false;
    private boolean isPaused  = false;
    private int activeScene   = 2;   // default: Bouncing Balls

    private Sidebar    sidebar;
    private StatsPanel statsPanel;

    private final List<double[]> trailPositions = new ArrayList<>();
    private final int MAX_TRAIL = 30;

    private long   lastFpsTime = 0;
    private int    frameCount  = 0;
    private int    currentFps  = 0;
    private double simTime     = 0.0;
    private static final double DT = 1.0 / 60.0;

    @Override
    public void start(Stage stage) {
        BorderPane root = new BorderPane();
        root.getStyleClass().add("root");

        // Header
        root.setTop(buildHeader());

        // Sidebar
        sidebar = new Sidebar();
        sidebar.setControls(new Sidebar.SimulationControls() {
            @Override public void onStart()           { startSimulation(); }
            @Override public void onPause()           { pauseSimulation(); }
            @Override public void onStop()            { stopSimulation(); }
            @Override public void onReset()           { resetSimulation(); }
            @Override public void onSceneSelected(int idx) { switchScene(idx); }
            @Override public void onAddBall()         { addRandomBall(); }
            @Override public void onDeleteSelected()  { /* future */ }
        });

        // Gravity slider → native engine
        sidebar.gravityProperty().addListener((obs, oldV, newV) -> {
            nativeWorld.setGravity(newV.doubleValue());
            statsPanel.setGravity(newV.doubleValue());
        });

        // Air resistance
        sidebar.airResistanceProperty().addListener((obs, oldV, newV) -> {
            nativeWorld.setAirResistance(newV.doubleValue());
        });

        // Elasticity (restitution)
        sidebar.elasticityProperty().addListener((obs, oldV, newV) -> {
            nativeWorld.setElasticity(newV.doubleValue());
        });

        // Friction
        sidebar.frictionProperty().addListener((obs, oldV, newV) -> {
            nativeWorld.setFriction(newV.doubleValue());
        });

        root.setLeft(sidebar);

        // Center canvas
        StackPane canvasHolder = new StackPane(canvas);
        canvasHolder.getStyleClass().add("simulation-view");
        canvasHolder.setAlignment(Pos.CENTER);
        canvas.widthProperty().bind(canvasHolder.widthProperty().subtract(4));
        canvas.heightProperty().bind(canvasHolder.heightProperty().subtract(4));
        root.setCenter(canvasHolder);

        // Bottom stats
        statsPanel = new StatsPanel();
        root.setBottom(statsPanel);

        // Scene
        Scene scene = new Scene(root, 1180, 720);
        var css = getClass().getResource("/style.css");
        if (css != null) {
            scene.getStylesheets().add(css.toExternalForm());
        } else {
            System.err.println("Warning: style.css not found on classpath");
        }

        stage.setTitle("Physics Simulator Pro");
        stage.setScene(scene);
        stage.setMinWidth(960);
        stage.setMinHeight(640);
        stage.show();

        // Animation loop
        timer = new AnimationTimer() {
            @Override
            public void handle(long now) {
                if (isRunning && !isPaused) {
                    double scale = sidebar.timeScaleProperty().get();
                    int steps = Math.max(1, (int) Math.round(2 * scale));
                    nativeWorld.step(steps);
                    simTime += DT * scale;
                }
                render(canvas.getGraphicsContext2D());
                updateStats(now);
            }
        };
        timer.start();

        // Start with Bouncing Balls
        switchScene(2);
        startSimulation();
    }

    // ════════════════════════════════════════════════════════════
    //  Header
    // ════════════════════════════════════════════════════════════

    private HBox buildHeader() {
        Label title = new Label("Physics Simulator Pro");
        title.getStyleClass().add("header-title");

        Label version = new Label("v2.0");
        version.getStyleClass().add("header-version");

        HBox header = new HBox(10, title, version);
        header.getStyleClass().add("header-bar");
        header.setAlignment(Pos.CENTER_LEFT);
        return header;
    }

    // ════════════════════════════════════════════════════════════
    //  Simulation control
    // ════════════════════════════════════════════════════════════

    private void startSimulation() {
        isRunning = true;
        isPaused  = false;
        sidebar.setRunningState(true, false);
    }

    private void pauseSimulation() {
        isPaused = true;
        sidebar.setRunningState(true, true);
    }

    private void stopSimulation() {
        isRunning = false;
        isPaused  = false;
        sidebar.setRunningState(false, false);
    }

    private void resetSimulation() {
        trailPositions.clear();
        simTime = 0.0;
        nativeWorld.createScene(activeScene);
        startSimulation();
    }

    private void switchScene(int idx) {
        activeScene = idx;
        trailPositions.clear();
        simTime = 0.0;
        nativeWorld.createScene(idx);
        isRunning = true;
        isPaused  = false;
        sidebar.setRunningState(true, false);
        sidebar.selectScene(idx);
    }

    private void addRandomBall() {
        try {
            nativeWorld.addRandomBall();
            if (nativeWorld.getBodyCount() > 14) {
                trailPositions.clear();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    // ════════════════════════════════════════════════════════════
    //  Rendering
    // ════════════════════════════════════════════════════════════

    private void render(GraphicsContext gc) {
        double w = canvas.getWidth();
        double h = canvas.getHeight();

        gc.setFill(Color.web("#181A1F"));
        gc.fillRect(0, 0, w, h);

        if (sidebar.showGridProperty().get()) {
            drawGrid(gc, w, h);
        }

        drawWorldBounds(gc);

        if (activeScene == 3) {
            renderPendulum(gc);
        } else if (activeScene == 4) {
            renderDoublePendulum(gc);
        } else {
            renderBodiesWithTrails(gc);
        }
    }

    private void drawGrid(GraphicsContext gc, double w, double h) {
        gc.setStroke(Color.web("#2B313A"));
        gc.setLineWidth(1.0);

        double ox = canvas.getWidth()  / 2.0;
        double oy = canvas.getHeight() / 2.0 + 40;

        double worldLeft   = -ox / SCALE;
        double worldRight  = (w - ox) / SCALE;
        double worldBottom = -(h - oy) / SCALE;
        double worldTop    = oy / SCALE;

        for (double x = Math.floor(worldLeft); x <= worldRight; x += 1.0) {
            double px = x * SCALE + ox;
            gc.strokeLine(px, 0, px, h);
        }
        for (double y = Math.floor(worldBottom); y <= worldTop; y += 1.0) {
            double py = -y * SCALE + oy;
            gc.strokeLine(0, py, w, py);
        }

        // Axes
        gc.setStroke(Color.web("#3B4450"));
        gc.setLineWidth(1.5);
        gc.strokeLine(ox, 0, ox, h);
        gc.strokeLine(0, oy, w, oy);
    }

    private void drawWorldBounds(GraphicsContext gc) {
        double[] wb = nativeWorld.getWorldBounds();
        if (wb == null || (wb[0] == 0 && wb[1] == 0)) return;

        double left   = worldToPixelX(wb[0]);
        double top    = worldToPixelY(wb[3]);
        double width  = (wb[1] - wb[0]) * SCALE;
        double height = (wb[3] - wb[2]) * SCALE;

        gc.setFill(Color.web("#1E222A"));
        gc.fillRect(left, top, width, height);

        gc.setStroke(Color.web("#4B5563"));
        gc.setLineWidth(2.0);
        gc.strokeRect(left, top, width, height);
    }

    private void renderBodiesWithTrails(GraphicsContext gc) {
        double[] positions = nativeWorld.getBodyPositions();
        int count = nativeWorld.getBodyCount();
        if (positions == null) return;

        if (sidebar.showTrailsProperty().get() && count > 1) {
            double[] pos = { positions[3], positions[4] };
            trailPositions.add(pos);
            if (trailPositions.size() > MAX_TRAIL) trailPositions.remove(0);
        } else if (!sidebar.showTrailsProperty().get()) {
            trailPositions.clear();
        }

        if (sidebar.showTrailsProperty().get() && trailPositions.size() > 1) {
            gc.setStroke(Color.web("#60A5FA", 0.45));
            gc.setLineWidth(2.0);
            for (int i = 1; i < trailPositions.size(); i++) {
                double[] p1 = trailPositions.get(i - 1);
                double[] p2 = trailPositions.get(i);
                gc.strokeLine(
                    worldToPixelX(p1[0]), worldToPixelY(p1[1]),
                    worldToPixelX(p2[0]), worldToPixelY(p2[1])
                );
            }
        }

        for (int i = 0; i < count; i++) {
            double wx = positions[i * 3];
            double wy = positions[i * 3 + 1];
            double px = worldToPixelX(wx);
            double py = worldToPixelY(wy);

            if (i == 0 && activeScene < 3) continue; // skip static ground

            double speed = Math.sqrt(wx * wx + wy * wy);
            double t = Math.min(speed / 25.0, 1.0);

            Color bodyColor = Color.color(0.2 + t * 0.55, 0.55 + t * 0.2, 1.0 - t * 0.35);
            gc.setFill(bodyColor);
            gc.fillOval(px - 10, py - 10, 20, 20);

            gc.setFill(Color.web("#FFFFFF", 0.35));
            gc.fillOval(px - 5, py - 7, 6, 6);
        }
    }

    private void renderPendulum(GraphicsContext gc) {
        double[] ps = nativeWorld.getPendulumState();
        if (ps == null) return;

        double pivPx = worldToPixelX(ps[0]);
        double pivPy = worldToPixelY(ps[1]);
        double bobPx = worldToPixelX(ps[2]);
        double bobPy = worldToPixelY(ps[3]);
        double omega = ps[5];

        gc.setFill(Color.web("#64748B"));
        gc.fillRect(pivPx - 22, pivPy - 14, 44, 14);

        gc.setStroke(Color.web("#94A3B8"));
        gc.setLineWidth(3.5);
        gc.strokeLine(pivPx, pivPy, bobPx, bobPy);

        double speed = Math.abs(omega);
        double t = Math.min(speed / 5.0, 1.0);
        Color bobColor = Color.color(0.25 + t * 0.65, 0.45 - t * 0.25, 1.0 - t * 0.55);
        gc.setFill(bobColor);
        gc.fillOval(bobPx - 15, bobPy - 15, 30, 30);

        gc.setFill(Color.web("#FFFFFF", 0.30));
        gc.fillOval(bobPx - 9, bobPy - 11, 9, 9);

        gc.setFill(Color.WHITE);
        gc.fillOval(pivPx - 6, pivPy - 6, 12, 12);

        gc.setFill(Color.web("#CBD5E1"));
        gc.setFont(javafx.scene.text.Font.font(13));
        double thetaDeg = Math.toDegrees(ps[4]);
        gc.fillText(String.format("θ = %.1f°", thetaDeg), worldToPixelX(6), worldToPixelY(7));
        gc.fillText(String.format("ω = %.2f rad/s", omega), worldToPixelX(6), worldToPixelY(5.5));
    }

    private void renderDoublePendulum(GraphicsContext gc) {
        double[] ps = nativeWorld.getDoublePendulumState();
        if (ps == null || ps.length < 10) return;

        double pivPx = worldToPixelX(ps[0]);
        double pivPy = worldToPixelY(ps[1]);
        double b1Px  = worldToPixelX(ps[2]);
        double b1Py  = worldToPixelY(ps[3]);
        double b2Px  = worldToPixelX(ps[4]);
        double b2Py  = worldToPixelY(ps[5]);

        gc.setStroke(Color.web("#A5B4FC"));
        gc.setLineWidth(5.0);
        gc.strokeLine(pivPx, pivPy, b1Px, b1Py);

        gc.setStroke(Color.web("#7DD3FC"));
        gc.setLineWidth(4.0);
        gc.strokeLine(b1Px, b1Py, b2Px, b2Py);

        gc.setFill(Color.web("#FBBF24"));
        gc.fillOval(b1Px - 14, b1Py - 14, 28, 28);

        gc.setFill(Color.web("#38BDF8"));
        gc.fillOval(b2Px - 13, b2Py - 13, 26, 26);

        gc.setFill(Color.WHITE);
        gc.fillOval(pivPx - 7, pivPy - 7, 14, 14);

        gc.setFill(Color.web("#E2E8F0"));
        gc.setFont(javafx.scene.text.Font.font(12));
        gc.fillText(String.format("θ1: %.1f°   ω1: %.2f",
                Math.toDegrees(ps[6]), ps[7]),
                worldToPixelX(-14), worldToPixelY(13));
        gc.fillText(String.format("θ2: %.1f°   ω2: %.2f",
                Math.toDegrees(ps[8]), ps[9]),
                worldToPixelX(-14), worldToPixelY(11));
    }

    // ════════════════════════════════════════════════════════════
    //  Stats / FPS
    // ════════════════════════════════════════════════════════════

    private void updateStats(long now) {
        frameCount++;
        if (lastFpsTime == 0) lastFpsTime = now;
        long elapsed = now - lastFpsTime;
        if (elapsed >= 1_000_000_000L) {
            currentFps = frameCount;
            frameCount = 0;
            lastFpsTime = now;
        }

        statsPanel.setSimulationTime(simTime);
        statsPanel.setFps(currentFps);
        statsPanel.setObjectCount(nativeWorld.getBodyCount());
        statsPanel.setGravity(sidebar.gravityProperty().get());

        // Live energy + collisions from native engine
        try {
            statsPanel.setKineticEnergy(nativeWorld.getKineticEnergy());
            statsPanel.setPotentialEnergy(nativeWorld.getPotentialEnergy());
            statsPanel.setTotalEnergy(nativeWorld.getTotalEnergy());
            statsPanel.setCollisions(nativeWorld.getCollisionCount());
        } catch (UnsatisfiedLinkError e) {
            // Native methods not yet available (old library)
        }

        statsPanel.setIntegrator("RK4");
        statsPanel.setTimeStep("1/60");
    }

    // ════════════════════════════════════════════════════════════
    //  Coordinate helpers
    // ════════════════════════════════════════════════════════════

    private double worldToPixelX(double wx) {
        double ox = canvas.getWidth() / 2.0;
        return wx * SCALE + ox;
    }

    private double worldToPixelY(double wy) {
        double oy = canvas.getHeight() / 2.0 + 40;
        return -wy * SCALE + oy;
    }

    public static void main(String[] args) {
        launch(args);
    }
}
