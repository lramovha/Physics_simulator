package com.physics.sim.ui;

import javafx.beans.property.BooleanProperty;
import javafx.beans.property.DoubleProperty;
import javafx.beans.property.IntegerProperty;
import javafx.beans.property.SimpleBooleanProperty;
import javafx.beans.property.SimpleDoubleProperty;
import javafx.beans.property.SimpleIntegerProperty;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.control.Button;
import javafx.scene.control.CheckBox;
import javafx.scene.control.Label;
import javafx.scene.control.ScrollPane;
import javafx.scene.control.Slider;
import javafx.scene.control.ToggleButton;
import javafx.scene.control.ToggleGroup;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Priority;
import javafx.scene.layout.Region;
import javafx.scene.layout.VBox;

/**
 * Left control panel.
 */
public class Sidebar extends VBox {

    public interface SimulationControls {
        void onStart();
        void onPause();
        void onStop();
        void onReset();
        void onSceneSelected(int sceneIndex);
        void onAddBall();
        void onDeleteSelected();
    }

    private SimulationControls controls;

    private final IntegerProperty selectedScene = new SimpleIntegerProperty(2);
    private final DoubleProperty  gravity       = new SimpleDoubleProperty(9.81);
    private final DoubleProperty  airResistance = new SimpleDoubleProperty(0.0);
    private final DoubleProperty  elasticity    = new SimpleDoubleProperty(0.8);
    private final DoubleProperty  friction      = new SimpleDoubleProperty(0.3);
    private final DoubleProperty  timeScale     = new SimpleDoubleProperty(1.0);

    private final BooleanProperty showGrid         = new SimpleBooleanProperty(true);
    private final BooleanProperty showVelocity     = new SimpleBooleanProperty(false);
    private final BooleanProperty showForces       = new SimpleBooleanProperty(false);
    private final BooleanProperty showTrails       = new SimpleBooleanProperty(true);
    private final BooleanProperty showCollisionPts = new SimpleBooleanProperty(false);

    private final ToggleGroup sceneGroup = new ToggleGroup();
    private final Button btnStart = new Button("▶  Start");
    private final Button btnPause = new Button("⏸  Pause");
    private final Button btnStop  = new Button("■  Stop");
    private final Button btnReset = new Button("↺  Reset");

    private final Slider gravitySlider;
    private final Slider airResSlider;
    private final Slider elasticitySlider;
    private final Slider frictionSlider;
    private final Slider timeScaleSlider;

    private final Label gravityValueLabel;
    private final Label airResValueLabel;
    private final Label elasticityValueLabel;
    private final Label frictionValueLabel;
    private final Label timeScaleValueLabel;

    public Sidebar() {
        getStyleClass().add("sidebar");
        setSpacing(10);
        setPadding(new Insets(8, 6, 8, 6));

        // ── Simulation section ───────────────────────────────────
        VBox simSection = section("SIMULATION");
        stylePrimary(btnStart);
        styleGhost(btnPause);
        styleDanger(btnStop);
        styleGhost(btnReset);

        btnStart.setMaxWidth(Double.MAX_VALUE);
        btnPause.setMaxWidth(Double.MAX_VALUE);
        btnStop.setMaxWidth(Double.MAX_VALUE);
        btnReset.setMaxWidth(Double.MAX_VALUE);

        HBox row1 = new HBox(6, btnStart, btnPause);
        HBox row2 = new HBox(6, btnStop,  btnReset);
        HBox.setHgrow(btnStart, Priority.ALWAYS);
        HBox.setHgrow(btnPause, Priority.ALWAYS);
        HBox.setHgrow(btnStop,  Priority.ALWAYS);
        HBox.setHgrow(btnReset, Priority.ALWAYS);
        simSection.getChildren().addAll(row1, row2);

        // ── Scene section ────────────────────────────────────────
        VBox sceneSection = section("SCENE");
        String[] sceneNames = {
            "Falling Objects",
            "Projectile Motion",
            "Bouncing Balls",
            "Pendulum",
            "Double Pendulum"
        };
        for (int i = 0; i < sceneNames.length; i++) {
            ToggleButton tb = new ToggleButton(sceneNames[i]);
            tb.setToggleGroup(sceneGroup);
            tb.getStyleClass().add("btn-scene");
            tb.setMaxWidth(Double.MAX_VALUE);
            tb.setUserData(i);
            if (i == 2) tb.setSelected(true);
            final int idx = i;
            tb.setOnAction(e -> {
                if (tb.isSelected()) {
                    selectedScene.set(idx);
                    if (controls != null) controls.onSceneSelected(idx);
                }
            });
            sceneSection.getChildren().add(tb);
        }

        // ── Physics section ──────────────────────────────────────
        VBox physSection = section("PHYSICS");

        gravitySlider    = makeSlider(0, 25, 9.81);
        airResSlider     = makeSlider(0, 2,  0.0);
        elasticitySlider = makeSlider(0, 1,  0.8);
        frictionSlider   = makeSlider(0, 1,  0.3);
        timeScaleSlider  = makeSlider(0.1, 3, 1.0);

        gravityValueLabel    = valueLabel("9.81");
        airResValueLabel     = valueLabel("0.00");
        elasticityValueLabel = valueLabel("0.80");
        frictionValueLabel   = valueLabel("0.30");
        timeScaleValueLabel  = valueLabel("1.00×");

        physSection.getChildren().addAll(
            labeledSlider("Gravity",        gravitySlider,    gravityValueLabel),
            labeledSlider("Air Resistance", airResSlider,     airResValueLabel),
            labeledSlider("Elasticity",     elasticitySlider, elasticityValueLabel),
            labeledSlider("Friction",       frictionSlider,   frictionValueLabel),
            labeledSlider("Time Scale",     timeScaleSlider,  timeScaleValueLabel)
        );

        gravitySlider.valueProperty().addListener((o, a, v) -> {
            gravity.set(v.doubleValue());
            gravityValueLabel.setText(String.format("%.2f", v.doubleValue()));
        });
        airResSlider.valueProperty().addListener((o, a, v) -> {
            airResistance.set(v.doubleValue());
            airResValueLabel.setText(String.format("%.2f", v.doubleValue()));
        });
        elasticitySlider.valueProperty().addListener((o, a, v) -> {
            elasticity.set(v.doubleValue());
            elasticityValueLabel.setText(String.format("%.2f", v.doubleValue()));
        });
        frictionSlider.valueProperty().addListener((o, a, v) -> {
            friction.set(v.doubleValue());
            frictionValueLabel.setText(String.format("%.2f", v.doubleValue()));
        });
        timeScaleSlider.valueProperty().addListener((o, a, v) -> {
            timeScale.set(v.doubleValue());
            timeScaleValueLabel.setText(String.format("%.2f×", v.doubleValue()));
        });

        // ── Objects section ──────────────────────────────────────
        VBox objSection = section("OBJECTS");
        Button btnAddBall = new Button("＋  Add Ball");
        Button btnAddBox  = new Button("＋  Add Box");
        Button btnDelete  = new Button("🗑  Delete Selected");
        stylePrimary(btnAddBall);
        styleGhost(btnAddBox);
        styleDanger(btnDelete);
        btnAddBall.setMaxWidth(Double.MAX_VALUE);
        btnAddBox.setMaxWidth(Double.MAX_VALUE);
        btnDelete.setMaxWidth(Double.MAX_VALUE);
        btnAddBox.setDisable(true); // future
        objSection.getChildren().addAll(btnAddBall, btnAddBox, btnDelete);

        // ── Rendering section ────────────────────────────────────
        VBox renderSection = section("RENDERING");
        CheckBox chkGrid       = new CheckBox("Grid");
        CheckBox chkVelocity   = new CheckBox("Velocity Vectors");
        CheckBox chkForces     = new CheckBox("Force Vectors");
        CheckBox chkTrails     = new CheckBox("Motion Trails");
        CheckBox chkCollisions = new CheckBox("Collision Points");

        chkGrid.setSelected(true);
        chkTrails.setSelected(true);

        chkGrid.selectedProperty().bindBidirectional(showGrid);
        chkVelocity.selectedProperty().bindBidirectional(showVelocity);
        chkForces.selectedProperty().bindBidirectional(showForces);
        chkTrails.selectedProperty().bindBidirectional(showTrails);
        chkCollisions.selectedProperty().bindBidirectional(showCollisionPts);

        renderSection.getChildren().addAll(
            chkGrid, chkVelocity, chkForces, chkTrails, chkCollisions
        );

        // ── Assemble ─────────────────────────────────────────────
        VBox content = new VBox(10,
            simSection, sceneSection, physSection, objSection, renderSection
        );
        content.setPadding(new Insets(4));

        ScrollPane scroll = new ScrollPane(content);
        scroll.setFitToWidth(true);
        scroll.setHbarPolicy(ScrollPane.ScrollBarPolicy.NEVER);
        scroll.setVbarPolicy(ScrollPane.ScrollBarPolicy.AS_NEEDED);
        scroll.getStyleClass().add("scroll-pane");
        VBox.setVgrow(scroll, Priority.ALWAYS);

        getChildren().add(scroll);

        // ── Wire buttons ─────────────────────────────────────────
        btnStart.setOnAction(e -> { if (controls != null) controls.onStart(); });
        btnPause.setOnAction(e -> { if (controls != null) controls.onPause(); });
        btnStop.setOnAction(e  -> { if (controls != null) controls.onStop();  });
        btnReset.setOnAction(e -> { if (controls != null) controls.onReset(); });
        btnAddBall.setOnAction(e -> { if (controls != null) controls.onAddBall(); });
        btnDelete.setOnAction(e  -> { if (controls != null) controls.onDeleteSelected(); });
    }

    // ── Helpers ──────────────────────────────────────────────────

    private VBox section(String title) {
        Label t = new Label(title);
        t.getStyleClass().add("sidebar-section-title");
        VBox box = new VBox(6, t);
        box.getStyleClass().add("sidebar-section");
        return box;
    }

    private Slider makeSlider(double min, double max, double value) {
        Slider s = new Slider(min, max, value);
        s.setShowTickLabels(false);
        s.setShowTickMarks(false);
        s.setPrefWidth(140);
        return s;
    }

    private Label valueLabel(String text) {
        Label l = new Label(text);
        l.getStyleClass().add("label-value");
        l.setMinWidth(42);
        l.setAlignment(Pos.CENTER_RIGHT);
        return l;
    }

    private VBox labeledSlider(String name, Slider slider, Label valueLbl) {
        Label nameLbl = new Label(name);
        nameLbl.getStyleClass().add("label-muted");
        HBox top = new HBox(6, nameLbl, new Region(), valueLbl);
        HBox.setHgrow(top.getChildren().get(1), Priority.ALWAYS);
        return new VBox(2, top, slider);
    }

    private void stylePrimary(Button b) { b.getStyleClass().addAll("btn", "btn-primary"); }
    private void styleGhost(Button b)   { b.getStyleClass().addAll("btn", "btn-ghost"); }
    private void styleDanger(Button b)  { b.getStyleClass().addAll("btn", "btn-danger"); }

    // ── Public API ───────────────────────────────────────────────

    public void setControls(SimulationControls c) {
        this.controls = c;
    }

    public IntegerProperty selectedSceneProperty() { return selectedScene; }
    public DoubleProperty  gravityProperty()       { return gravity; }
    public DoubleProperty  airResistanceProperty() { return airResistance; }
    public DoubleProperty  elasticityProperty()    { return elasticity; }
    public DoubleProperty  frictionProperty()      { return friction; }
    public DoubleProperty  timeScaleProperty()     { return timeScale; }

    public BooleanProperty showGridProperty()         { return showGrid; }
    public BooleanProperty showVelocityProperty()     { return showVelocity; }
    public BooleanProperty showForcesProperty()       { return showForces; }
    public BooleanProperty showTrailsProperty()       { return showTrails; }
    public BooleanProperty showCollisionPtsProperty() { return showCollisionPts; }

    public void selectScene(int index) {
        for (var t : sceneGroup.getToggles()) {
            if (t.getUserData() != null && (int) t.getUserData() == index) {
                sceneGroup.selectToggle(t);
                selectedScene.set(index);
                break;
            }
        }
    }

    public void setRunningState(boolean running, boolean paused) {
        btnStart.setDisable(running && !paused);
        btnPause.setDisable(!running || paused);
    }
}