package com.physics.sim.ui;

import javafx.geometry.Pos;
import javafx.scene.control.Label;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;

/**
 * Bottom live-statistics bar.
 */
public class StatsPanel extends HBox {

    private final Label valTime;
    private final Label valFps;
    private final Label valObjects;
    private final Label valGravity;
    private final Label valKinetic;
    private final Label valPotential;
    private final Label valTotal;
    private final Label valCollisions;
    private final Label valIntegrator;
    private final Label valTimeStep;

    public StatsPanel() {
        getStyleClass().add("stats-panel");
        setAlignment(Pos.CENTER_LEFT);
        setSpacing(10);

        valTime       = createValueLabel("0.00 s");
        valFps        = createValueLabel("0");
        valObjects    = createValueLabel("0");
        valGravity    = createValueLabel("9.81");
        valKinetic    = createValueLabel("—");
        valPotential  = createValueLabel("—");
        valTotal      = createValueLabel("—");
        valCollisions = createValueLabel("0");
        valIntegrator = createValueLabel("RK4");
        valTimeStep   = createValueLabel("1/60");

        getChildren().addAll(
            makeCard("SIM TIME",    valTime),
            makeCard("FPS",         valFps),
            makeCard("OBJECTS",     valObjects),
            makeCard("GRAVITY",     valGravity),
            makeCard("KINETIC E",   valKinetic),
            makeCard("POTENTIAL E", valPotential),
            makeCard("TOTAL E",     valTotal),
            makeCard("COLLISIONS",  valCollisions),
            makeCard("INTEGRATOR",  valIntegrator),
            makeCard("TIME STEP",   valTimeStep)
        );
    }

    private VBox makeCard(String title, Label valueLabel) {
        Label titleLbl = new Label(title);
        titleLbl.getStyleClass().add("stat-label");

        VBox card = new VBox(2, titleLbl, valueLabel);
        card.getStyleClass().add("stat-card");
        card.setAlignment(Pos.CENTER);
        return card;
    }

    private Label createValueLabel(String initial) {
        Label l = new Label(initial);
        l.getStyleClass().add("stat-value");
        return l;
    }

    // ── Public update API ────────────────────────────────────────

    public void setSimulationTime(double seconds) {
        valTime.setText(String.format("%.2f s", seconds));
    }

    public void setFps(int fps) {
        valFps.setText(String.valueOf(fps));
        if (fps >= 55) {
            valFps.getStyleClass().removeAll("warning");
            if (!valFps.getStyleClass().contains("success"))
                valFps.getStyleClass().add("success");
        } else if (fps >= 30) {
            valFps.getStyleClass().removeAll("success", "warning");
        } else {
            valFps.getStyleClass().removeAll("success");
            if (!valFps.getStyleClass().contains("warning"))
                valFps.getStyleClass().add("warning");
        }
    }

    public void setObjectCount(int count) {
        valObjects.setText(String.valueOf(count));
    }

    public void setGravity(double g) {
        valGravity.setText(String.format("%.2f", g));
    }

    public void setKineticEnergy(double ke) {
        valKinetic.setText(String.format("%.1f", ke));
    }

    public void setPotentialEnergy(double pe) {
        valPotential.setText(String.format("%.1f", pe));
    }

    public void setTotalEnergy(double te) {
        valTotal.setText(String.format("%.1f", te));
    }

    public void setCollisions(int n) {
        valCollisions.setText(String.valueOf(n));
    }

    public void setIntegrator(String name) {
        valIntegrator.setText(name);
    }

    public void setTimeStep(String step) {
        valTimeStep.setText(step);
    }
}