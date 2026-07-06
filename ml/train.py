import joblib
import pandas as pd
import shap
import matplotlib.pyplot as plt

from pathlib import Path

from sklearn.model_selection import train_test_split
from sklearn.metrics import (
    accuracy_score,
    precision_score,
    recall_score,
    f1_score,
    roc_auc_score,
    confusion_matrix,
    classification_report,
    ConfusionMatrixDisplay,
    RocCurveDisplay,
)


from xgboost import XGBClassifier


def main():

    # ==========================================================
    # Load Dataset
    # ==========================================================

    input_path = "data/processed_dataset.csv"

    df = pd.read_csv(input_path)

    print("=" * 60)
    print("GenomeFlowAI - Model Training")
    print("=" * 60)

    print(f"\nDataset Shape : {df.shape}")

    # ==========================================================
    # Feature Selection
    # ==========================================================

    feature_columns = [
        "generation",
        "clone_size",
        "clone_fraction",
        "average_fitness",
        "average_age",
        "average_passenger_mutations",
        "driver_mutation_count",
    ]

    X = df[feature_columns]

    y = df["aggressive"]

    print("\nFeatures")
    print("-" * 60)

    for feature in feature_columns:
        print(feature)

    # ==========================================================
    # Train/Test Split
    # ==========================================================

    X_train, X_test, y_train, y_test = train_test_split(
        X,
        y,
        test_size=0.20,
        random_state=42,
        stratify=y,
    )

    print("\nTraining Samples :", len(X_train))
    print("Testing Samples  :", len(X_test))

    # ==========================================================
    # Train XGBoost
    # ==========================================================

    model = XGBClassifier(
        objective="binary:logistic",
        n_estimators=100,
        max_depth=5,
        learning_rate=0.1,
        random_state=42,
        eval_metric="logloss",
    )

    model.fit(X_train, y_train)

    print("\nModel training completed.")

    # ==========================================================
    # Prediction
    # ==========================================================

    y_pred = model.predict(X_test)

    y_prob = model.predict_proba(X_test)[:, 1]

    # ==========================================================
    # Evaluation
    # ==========================================================

    accuracy = accuracy_score(y_test, y_pred)

    precision = precision_score(y_test, y_pred)

    recall = recall_score(y_test, y_pred)

    f1 = f1_score(y_test, y_pred)

    roc_auc = roc_auc_score(y_test, y_prob)

    print("\nEvaluation Metrics")
    print("-" * 60)

    print(f"Accuracy  : {accuracy:.4f}")
    print(f"Precision : {precision:.4f}")
    print(f"Recall    : {recall:.4f}")
    print(f"F1 Score  : {f1:.4f}")
    print(f"ROC AUC   : {roc_auc:.4f}")

    print("\nClassification Report")
    print("-" * 60)

    print(classification_report(y_test, y_pred))

    print("\nConfusion Matrix")
    print("-" * 60)

    print(confusion_matrix(y_test, y_pred))

    # ==========================================================
    # Feature Importance
    # ==========================================================

    plt.figure(figsize=(8,5))

    plt.barh(
        feature_columns,
        model.feature_importances_
    )

    plt.xlabel("Importance")
    plt.title("Feature Importance")

    plt.tight_layout()

    plt.savefig(
        "reports/feature_importance.png",
        dpi=300
    )

    plt.close()

    # ==========================================================
    # Confusion Matrix Figure
    # ==========================================================

    disp = ConfusionMatrixDisplay(
        confusion_matrix=confusion_matrix(
            y_test,
            y_pred
        )
    )

    disp.plot()

    plt.title("Confusion Matrix")

    plt.tight_layout()

    plt.savefig(
        "reports/confusion_matrix.png",
        dpi=300
    )

    plt.close()


    # ==========================================================
    # ROC Curve
    # ==========================================================

    RocCurveDisplay.from_predictions(
        y_test,
        y_prob
    )

    plt.title("ROC Curve")

    plt.tight_layout()

    plt.savefig(
        "reports/roc_curve.png",
        dpi=300
    )

    plt.close()

    # ==========================================================
    # SHAP
    # ==========================================================

    explainer = shap.TreeExplainer(model)

    shap_values = explainer.shap_values(X_test)

    plt.figure()

    shap.summary_plot(
        shap_values,
        X_test,
        show=False
    )

    plt.tight_layout()

    plt.savefig(
        "reports/shap_summary.png",
        dpi=300
    )

    plt.close()

    model_path = "models/xgboost_model.joblib"

    joblib.dump(
        model,
        model_path
    )

    # ==========================================================
    # Save Model
    # ==========================================================

    Path("models").mkdir(exist_ok=True)
    Path("reports").mkdir(exist_ok=True)

    

    model_path = "models/xgboost_model.joblib"

    joblib.dump(model, model_path)

    print("\nModel saved successfully.")
    print(f"Location : {model_path}")

    print("\nReports Generated")
    print("-" * 60)

    print("reports/feature_importance.png")
    print("reports/confusion_matrix.png")
    print("reports/roc_curve.png")
    print("reports/shap_summary.png")


if __name__ == "__main__":
    main()