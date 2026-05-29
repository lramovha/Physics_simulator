package com.physics.sim;

import com.physics.sim.nativebridge.NativePhysicsWorld;
import javafx.application.Application;
import javafx.animation.AnimationTimer;
import javafx.scene.Scene;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.control.Button;
import javafx.scene.control.ComboBox;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.HBox;
import javafx.scene.paint.Color;
import javafx.stage.Stage;

public class MainApp extends Application {

    private NativePhysicsWorld nativeWorld = new NativePhysicsWorld();
    private Canvas canvas = new Canvas(800, 600);
    private AnimationTimer timer;
    private boolean isRunning = false;

    @Override
    public void start(Stage stage) {
        BorderPane root = new BorderPane();
        GraphicsContext gc = canvas.getGraphicsContext2D();

        // Controls
        ComboBox<String> sceneSelector = new ComboBox<>();
        sceneSelector.getItems().addAll("Falling Balls", "Projectile", "Bouncing Balls");
        sceneSelector.setValue("Falling Balls");

        Button btnStart = new Button("Start");
        Button btnPause = new Button("Pause");
        Button btnReset = new Button("Reset");

        HBox controls = new HBox(10, sceneSelector, btnStart, btnPause, btnReset);
        controls.setStyle("-fx-padding: 10;");

        root.setTop(controls);
        root.setCenter(canvas);

        // Button actions
        btnStart.setOnAction(e -> {
            isRunning = true;
            nativeWorld.createScene(sceneSelector.getSelectionModel().getSelectedIndex());
        });

        btnPause.setOnAction(e -> isRunning = false);
        btnReset.setOnAction(e -> nativeWorld.createScene(sceneSelector.getSelectionModel().getSelectedIndex()));

        Scene scene = new Scene(root, 820, 650);
        stage.setTitle("Physics Simulator - JavaFX");
        stage.setScene(scene);
        stage.show();

        // Animation Loop
        timer = new AnimationTimer() {
            @Override
            public void handle(long now) {
                if (isRunning) {
                    nativeWorld.step(2);  // Step simulation
                }
                render(gc);
            }
        };
        timer.start();
    }

    private void render(GraphicsContext gc) {
        gc.setFill(Color.BLACK);
        gc.fillRect(0, 0, canvas.getWidth(), canvas.getHeight());

        double[] positions = nativeWorld.getBodyPositions();
        int count = nativeWorld.getBodyCount();

        gc.setFill(Color.CYAN);
        for (int i = 0; i < count; i++) {
            double x = positions[i*3] * 15 + 400;      // Scale and center
            double y = -positions[i*3 + 1] * 15 + 300; // Flip Y axis
            gc.fillOval(x - 8, y - 8, 16, 16);
        }
    }

    public static void main(String[] args) {
        launch(args);
    }
}