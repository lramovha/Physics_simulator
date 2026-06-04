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
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.HBox;
import javafx.scene.paint.Color;
import javafx.stage.Stage;

/**
 * MainApp — JavaFX frontend for the Physics Simulator.
 *
 * Coordinate system:
 *   Physics world uses metres, Y-up, origin at the canvas centre.
 *   Canvas uses pixels, Y-down (JavaFX standard).
 *
 *   pixel_x =  world_x * SCALE + ORIGIN_X
 *   pixel_y = -world_y * SCALE + ORIGIN_Y   (Y axis flip)
 */
public class MainApp extends Application {

    // ── Coordinate conversion constants ─────────────────────────
    private static final double SCALE    = 15.0;   // pixels per metre
    private static final double ORIGIN_X = 400.0;  // canvas centre X
    private static final double ORIGIN_Y = 350.0;  // canvas centre Y

    // ── JavaFX nodes ────────────────────────────────────────────
    private final NativePhysicsWorld nativeWorld = new NativePhysicsWorld();
    private final Canvas             canvas      = new Canvas(800, 650);
    private       AnimationTimer     timer;
    private       boolean            isRunning   = false;

    // Keep a reference so render() knows which scene is active
    private       int                activeScene = 0;

    // ── Status label ─────────────────────────────────────────────
    private final Label lblStatus = new Label("Press Start");

    @Override
    public void start(Stage stage) {
        BorderPane root = new BorderPane();
        GraphicsContext gc = canvas.getGraphicsContext2D();

        // ── Scene selector ──────────────────────────────────────
        ComboBox<String> sceneSelector = new ComboBox<>();
        sceneSelector.getItems().addAll(
            "Falling Balls",      // index 0
            "Projectile Motion",  // index 1
            "Bouncing Balls",     // index 2
            "Simple Pendulum",     // index 3
            "Double Pendulum"     // index 4
        );
        sceneSelector.setValue("Falling Balls");

        // ── Buttons ─────────────────────────────────────────────
        Button btnStart = new Button("▶  Start");
        Button btnPause = new Button("⏸  Pause");
        Button btnReset = new Button("↺  Reset");

        HBox controls = new HBox(10,
            sceneSelector, btnStart, btnPause, btnReset, lblStatus);
        controls.setStyle("-fx-padding: 10; -fx-alignment: CENTER_LEFT;");

        root.setTop(controls);
        root.setCenter(canvas);

        // ── Button actions ───────────────────────────────────────
        btnStart.setOnAction(e -> {
            activeScene = sceneSelector.getSelectionModel().getSelectedIndex();
            nativeWorld.createScene(activeScene);
            isRunning = true;
            lblStatus.setText("Running: " + sceneSelector.getValue());
        });

        btnPause.setOnAction(e -> {
            isRunning = false;
            lblStatus.setText("Paused");
        });

        btnReset.setOnAction(e -> {
            activeScene = sceneSelector.getSelectionModel().getSelectedIndex();
            nativeWorld.createScene(activeScene);
            isRunning = true;
            lblStatus.setText("Reset: " + sceneSelector.getValue());
        });

        // ── Animation loop ───────────────────────────────────────
        timer = new AnimationTimer() {
            @Override
            public void handle(long now) {
                if (isRunning) {
                    nativeWorld.step(2);
                }
                render(gc);
            }
        };
        timer.start();

        // ── Stage ────────────────────────────────────────────────
        Scene scene = new Scene(root, 820, 700);
        stage.setTitle("Physics Simulator");
        stage.setScene(scene);
        stage.show();
    }

    // ─────────────────────────────────────────────────────────────
    // render()
    // Called every frame whether paused or running.
    // ─────────────────────────────────────────────────────────────
    private void render(GraphicsContext gc) {

        // ── Background ──────────────────────────────────────────
        gc.setFill(Color.color(0.08, 0.08, 0.12));
        gc.fillRect(0, 0, canvas.getWidth(), canvas.getHeight());

        // ── World boundary box ───────────────────────────────────
        double[] wb = nativeWorld.getWorldBounds();
        // wb = [minX, maxX, minY, maxY, minZ, maxZ]
        // Only draw if bounds are active (not all zeroes)
        if (wb != null && (wb[0] != 0 || wb[1] != 0)) {
            double bLeft   = worldToPixelX(wb[0]);
            double bTop    = worldToPixelY(wb[3]);   // maxY → top of screen
            double bWidth  = (wb[1] - wb[0]) * SCALE;
            double bHeight = (wb[3] - wb[2]) * SCALE;

            // Filled background for the world area
            gc.setFill(Color.color(0.12, 0.14, 0.20));
            gc.fillRect(bLeft, bTop, bWidth, bHeight);

            // Border
            gc.setStroke(Color.color(0.35, 0.45, 0.65));
            gc.setLineWidth(2.0);
            gc.strokeRect(bLeft, bTop, bWidth, bHeight);

            // Floor line (minY)
            double floorY = worldToPixelY(wb[2]);
            gc.setStroke(Color.color(0.50, 0.55, 0.70, 0.7));
            gc.setLineWidth(1.5);
            gc.strokeLine(bLeft, floorY, bLeft + bWidth, floorY);
        }

        // ── Scene-specific rendering ─────────────────────────────
        if (activeScene == 3) {
        renderPendulum(gc);
        } else if (activeScene == 4) {
        renderDoublePendulum(gc);
        } else {
        renderBodies(gc);
        }
    }

    // ─────────────────────────────────────────────────────────────
    // renderBodies — generic ball renderer for scenes 0, 1, 2
    // ─────────────────────────────────────────────────────────────
    private void renderBodies(GraphicsContext gc) {
        double[] positions = nativeWorld.getBodyPositions();
        int      count     = nativeWorld.getBodyCount();
        if (positions == null || count == 0) return;

        for (int i = 0; i < count; i++) {
            double wx = positions[i * 3];
            double wy = positions[i * 3 + 1];
            double px = worldToPixelX(wx);
            double py = worldToPixelY(wy);

            // Skip rendering static ground bodies (they create the big box)
            if (i == 0 && activeScene != 3 && activeScene != 4) continue;

            // Draw dynamic balls
            double speed = Math.sqrt(
                positions[i*3]*positions[i*3] + 
                positions[i*3+1]*positions[i*3+1]);
            double t = Math.min(speed / 20.0, 1.0);
            Color ballColor = Color.color(0.1 + t * 0.3, 0.7 - t * 0.4, 1.0);

            gc.setFill(ballColor);
            gc.fillOval(px - 8, py - 8, 16, 16);

            // if (i == 0) {
            //     // First body is always the static ground/floor — draw as a bar
            //     gc.setFill(Color.color(0.50, 0.55, 0.70, 0.5));
            //     gc.fillRect(px - 200, py - 4, 400, 8);
            // } else {
            //     // Dynamic balls — colour shifts from cyan (fast) to blue (slow)
            //     double speed = Math.sqrt(
            //         positions[i*3]*positions[i*3] +
            //         positions[i*3+1]*positions[i*3+1]);
            //     double t = Math.min(speed / 20.0, 1.0);
            //     Color ballColor = Color.color(
            //         0.1 + t * 0.3,
            //         0.7 - t * 0.4,
            //         1.0
            //     );
            //     gc.setFill(ballColor);
            //     gc.fillOval(px - 8, py - 8, 16, 16);

            //     // Subtle highlight
            //     gc.setFill(Color.color(1, 1, 1, 0.25));
            //     gc.fillOval(px - 5, py - 7, 5, 5);
            // }
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


    // ─────────────────────────────────────────────────────────────
    // Coordinate helpers
    // ─────────────────────────────────────────────────────────────
    private double worldToPixelX(double wx) {
        return wx * SCALE + ORIGIN_X;
    }

    private double worldToPixelY(double wy) {
        return -wy * SCALE + ORIGIN_Y;   // Y-axis flip
    }

    public static void main(String[] args) {
        launch(args);
    }
}


// package com.physics.sim;

// import com.physics.sim.nativebridge.NativePhysicsWorld;
// import javafx.application.Application;
// import javafx.animation.AnimationTimer;
// import javafx.scene.Scene;
// import javafx.scene.canvas.Canvas;
// import javafx.scene.canvas.GraphicsContext;
// import javafx.scene.control.Button;
// import javafx.scene.control.ComboBox;
// import javafx.scene.layout.BorderPane;
// import javafx.scene.layout.HBox;
// import javafx.scene.paint.Color;
// import javafx.stage.Stage;

// public class MainApp extends Application {

//     private NativePhysicsWorld nativeWorld = new NativePhysicsWorld();
//     private Canvas canvas = new Canvas(800, 600);
//     private AnimationTimer timer;
//     private boolean isRunning = false;

//     @Override
//     public void start(Stage stage) {
//         BorderPane root = new BorderPane();
//         GraphicsContext gc = canvas.getGraphicsContext2D();

//         // Controls
//         ComboBox<String> sceneSelector = new ComboBox<>();
//         sceneSelector.getItems().addAll("Falling Balls", "Projectile", "Bouncing Balls");
//         sceneSelector.setValue("Falling Balls");

//         Button btnStart = new Button("Start");
//         Button btnPause = new Button("Pause");
//         Button btnReset = new Button("Reset");

//         HBox controls = new HBox(10, sceneSelector, btnStart, btnPause, btnReset);
//         controls.setStyle("-fx-padding: 10;");

//         root.setTop(controls);
//         root.setCenter(canvas);

//         // Button actions
//         btnStart.setOnAction(e -> {
//             isRunning = true;
//             nativeWorld.createScene(sceneSelector.getSelectionModel().getSelectedIndex());
//         });

//         btnPause.setOnAction(e -> isRunning = false);
//         btnReset.setOnAction(e -> nativeWorld.createScene(sceneSelector.getSelectionModel().getSelectedIndex()));

//         Scene scene = new Scene(root, 820, 650);
//         stage.setTitle("Physics Simulator - JavaFX");
//         stage.setScene(scene);
//         stage.show();

//         // Animation Loop
//         timer = new AnimationTimer() {
//             @Override
//             public void handle(long now) {
//                 if (isRunning) {
//                     nativeWorld.step(2);  // Step simulation
//                 }
//                 render(gc);
//             }
//         };
//         timer.start();
//     }

//     private void render(GraphicsContext gc) {
//         gc.setFill(Color.BLACK);
//         gc.fillRect(0, 0, canvas.getWidth(), canvas.getHeight());

//         double[] positions = nativeWorld.getBodyPositions();
//         int count = nativeWorld.getBodyCount();

//         gc.setFill(Color.CYAN);
//         for (int i = 0; i < count; i++) {
//             double x = positions[i*3] * 15 + 400;      // Scale and center
//             double y = -positions[i*3 + 1] * 15 + 300; // Flip Y axis
//             gc.fillOval(x - 8, y - 8, 16, 16);
//         }
//     }

//     public static void main(String[] args) {
//         launch(args);
//     }
// }