package com.physics.sim;

import com.physics.sim.nativebridge.NativePhysicsWorld;
import javafx.application.Application;
import javafx.animation.AnimationTimer;
import javafx.scene.Scene;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.control.Button;
import javafx.scene.control.ComboBox;
import javafx.scene.control.Label;
import javafx.scene.control.Slider;           // slider
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.HBox;
import javafx.scene.paint.Color;
import javafx.stage.Stage;
import java.util.ArrayList;
import java.util.List;

public class MainApp extends Application {

    private static final double SCALE = 15.0;
    private static final double ORIGIN_X = 400.0;
    private static final double ORIGIN_Y = 350.0;

    private final NativePhysicsWorld nativeWorld = new NativePhysicsWorld();
    private final Canvas canvas = new Canvas(800, 650);
    private AnimationTimer timer;
    private boolean isRunning = false;
    private int activeScene = 0;

    // Controls
    private ComboBox<String> sceneSelector;
    private Slider gravitySlider;
    private Button btnAddBall;
    private Label lblStatus;

    // Trails
    private final List<double[]> trailPositions = new ArrayList<>();
    private final int MAX_TRAIL = 25;

    @Override
    public void start(Stage stage) {
        BorderPane root = new BorderPane();
        GraphicsContext gc = canvas.getGraphicsContext2D();

        // Scene Selector
        sceneSelector = new ComboBox<>();
        sceneSelector.getItems().addAll(
            "Falling Balls", "Projectile Motion", "Bouncing Balls",
            "Simple Pendulum", "Double Pendulum"
        );
        sceneSelector.setValue("Bouncing Balls");

        // Gravity Slider
        gravitySlider = new Slider(0, 25, 9.81);
        gravitySlider.setShowTickLabels(true);
        gravitySlider.setShowTickMarks(true);
        gravitySlider.setPrefWidth(200);

        // Connect slider to native engine
        gravitySlider.valueProperty().addListener((obs, oldVal, newVal) -> {
            nativeWorld.setGravity(newVal.doubleValue());
        });

        btnAddBall = new Button("➕ Add Ball");
        btnAddBall.setOnAction(e -> addRandomBall());

        lblStatus = new Label("Ready");

        HBox controls = new HBox(12, sceneSelector, gravitySlider, btnAddBall, lblStatus);
        controls.setStyle("-fx-padding: 12; -fx-alignment: CENTER_LEFT;");

        root.setTop(controls);
        root.setCenter(canvas);

        // Button actions
        sceneSelector.setOnAction(e -> {
            activeScene = sceneSelector.getSelectionModel().getSelectedIndex();
            nativeWorld.createScene(activeScene);
            isRunning = true;
            lblStatus.setText("Running: " + sceneSelector.getValue());
            trailPositions.clear();
        });

        // Animation Loop
        timer = new AnimationTimer() {
            @Override
            public void handle(long now) {
                if (isRunning) {
                    nativeWorld.step(2);
                }
                render(gc);
                updateStatus();
            }
        };
        timer.start();

        Scene scene = new Scene(root, 820, 700);
        stage.setTitle("Physics Simulator");
        stage.setScene(scene);
        stage.show();

        // Initial scene
        nativeWorld.createScene(2); // Start with Bouncing Balls
        isRunning = true;
    }

    private void render(GraphicsContext gc) {
        // Background
        gc.setFill(Color.color(0.06, 0.06, 0.11));
        gc.fillRect(0, 0, canvas.getWidth(), canvas.getHeight());

        // World bounds
        drawWorldBounds(gc);

        if (activeScene == 3) {
            renderPendulum(gc);
        } else if (activeScene == 4) {
            renderDoublePendulum(gc);
        } else {
            renderBodiesWithTrails(gc);
        }
    }

    private void drawWorldBounds(GraphicsContext gc) {
        double[] wb = nativeWorld.getWorldBounds();
        if (wb == null || wb[0] == 0) return;

        double left = worldToPixelX(wb[0]);
        double top = worldToPixelY(wb[3]);
        double width = (wb[1] - wb[0]) * SCALE;
        double height = (wb[3] - wb[2]) * SCALE;

        gc.setFill(Color.color(0.12, 0.14, 0.20));
        gc.fillRect(left, top, width, height);

        gc.setStroke(Color.color(0.4, 0.5, 0.7));
        gc.setLineWidth(2.5);
        gc.strokeRect(left, top, width, height);
    }

    private void renderBodiesWithTrails(GraphicsContext gc) {
        double[] positions = nativeWorld.getBodyPositions();
        int count = nativeWorld.getBodyCount();
        if (positions == null) return;

        // Update trail (use first dynamic ball)
        if (count > 1) {
            double[] pos = {positions[3], positions[4]}; // second body
            trailPositions.add(pos);
            if (trailPositions.size() > MAX_TRAIL) trailPositions.remove(0);
        }

        // Draw trails
        gc.setStroke(Color.color(0.4, 0.8, 1.0, 0.4));
        gc.setLineWidth(2.5);
        for (int i = 1; i < trailPositions.size(); i++) {
            double[] p1 = trailPositions.get(i-1);
            double[] p2 = trailPositions.get(i);
            gc.strokeLine(worldToPixelX(p1[0]), worldToPixelY(p1[1]),
                         worldToPixelX(p2[0]), worldToPixelY(p2[1]));
        }

        // Draw bodies
        for (int i = 0; i < count; i++) {
            double wx = positions[i * 3];
            double wy = positions[i * 3 + 1];
            double px = worldToPixelX(wx);
            double py = worldToPixelY(wy);

            if (i == 0 && activeScene < 3) continue; // Skip static ground

            double speed = Math.sqrt(wx*wx + wy*wy);
            double t = Math.min(speed / 25.0, 1.0);

            gc.setFill(Color.color(0.2 + t*0.6, 0.6, 1.0 - t*0.3));
            gc.fillOval(px - 9, py - 9, 18, 18);

            // Highlight
            gc.setFill(Color.WHITE);
            gc.fillOval(px - 4, py - 7, 5, 5);
        }
    }

    // ─────────────────────────────────────────────────────────────
    // renderPendulum — draws pivot, rod, and bob for scene 3
    // ─────────────────────────────────────────────────────────────
    private void renderPendulum(GraphicsContext gc) {
        double[] ps = nativeWorld.getPendulumState();
        // ps = [pivotX, pivotY, bobX, bobY, theta, omega]
        if (ps == null) return;

        double pivPx = worldToPixelX(ps[0]);
        double pivPy = worldToPixelY(ps[1]);
        double bobPx = worldToPixelX(ps[2]);
        double bobPy = worldToPixelY(ps[3]);
        double omega  = ps[5];

        // ── Pivot mount bracket ──────────────────────────────────
        gc.setFill(Color.color(0.6, 0.6, 0.7));
        gc.fillRect(pivPx - 20, pivPy - 12, 40, 12);

        // ── Rod ──────────────────────────────────────────────────
        gc.setStroke(Color.color(0.75, 0.78, 0.85));
        gc.setLineWidth(3.0);
        gc.strokeLine(pivPx, pivPy, bobPx, bobPy);

        // ── Bob — colour indicates speed (blue=slow, orange=fast) ─
        double speed = Math.abs(omega);
        double t     = Math.min(speed / 5.0, 1.0);
        Color bobColor = Color.color(0.2 + t * 0.7, 0.4 - t * 0.3, 1.0 - t * 0.6);

        gc.setFill(bobColor);
        gc.fillOval(bobPx - 14, bobPy - 14, 28, 28);

        // Bob highlight
        gc.setFill(Color.color(1, 1, 1, 0.30));
        gc.fillOval(bobPx - 9, bobPy - 11, 8, 8);

        // ── Pivot pin ────────────────────────────────────────────
        gc.setFill(Color.WHITE);
        gc.fillOval(pivPx - 5, pivPy - 5, 10, 10);

        // ── Angle label ──────────────────────────────────────────
        double thetaDeg = Math.toDegrees(ps[4]);
        gc.setFill(Color.color(0.75, 0.85, 1.0));
        gc.fillText(String.format("θ = %.1f°", thetaDeg),
                    worldToPixelX(6), worldToPixelY(6));
        gc.fillText(String.format("ω = %.2f rad/s", omega),
                    worldToPixelX(6), worldToPixelY(4.5));
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

        // Upper rod
        gc.setStroke(Color.rgb(180, 180, 220));
        gc.setLineWidth(5.0);
        gc.strokeLine(pivPx, pivPy, b1Px, b1Py);

        // Lower rod
        gc.setStroke(Color.rgb(160, 200, 240));
        gc.setLineWidth(4.0);
        gc.strokeLine(b1Px, b1Py, b2Px, b2Py);

        // Bob 1 (upper)
        gc.setFill(Color.rgb(240, 160, 80));
        gc.fillOval(b1Px - 13, b1Py - 13, 26, 26);

        // Bob 2 (lower)
        gc.setFill(Color.rgb(80, 180, 240));
        gc.fillOval(b2Px - 12, b2Py - 12, 24, 24);

        // Pivot
        gc.setFill(Color.WHITE);
        gc.fillOval(pivPx - 7, pivPy - 7, 14, 14);

        // Info text
        gc.setFill(Color.WHITE);
        gc.setFont(javafx.scene.text.Font.font(12));
        gc.fillText(String.format("θ1: %.1f°  ω1: %.2f", Math.toDegrees(ps[6]), ps[7]), 
                    worldToPixelX(-12), worldToPixelY(12));
        gc.fillText(String.format("θ2: %.1f°  ω2: %.2f", Math.toDegrees(ps[8]), ps[9]), 
                    worldToPixelX(-12), worldToPixelY(10));
    }

    private void addRandomBall() {
        try {
            nativeWorld.addRandomBall();
            lblStatus.setText("➕ Ball added!");
            
            // Auto clear old trails when too many balls
            if (nativeWorld.getBodyCount() > 12) {
                trailPositions.clear();
            }
        } catch (Exception e) {
            lblStatus.setText("Error adding ball");
            e.printStackTrace();
        }
    }
    
    private void updateStatus() {
        int count = nativeWorld.getBodyCount();
        lblStatus.setText(String.format("Bodies: %d | Gravity: %.2f m/s²", 
                          count, gravitySlider.getValue()));
    }

    private double worldToPixelX(double wx) { return wx * SCALE + ORIGIN_X; }
    private double worldToPixelY(double wy) { return -wy * SCALE + ORIGIN_Y; }

    public static void main(String[] args) {
        launch(args);
    }
}

