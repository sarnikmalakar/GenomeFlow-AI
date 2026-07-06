import pandas as pd


def main():

    # ==========================================================
    # Load Dataset
    # ==========================================================

    input_path = "data/clone_dataset.csv"

    df = pd.read_csv(input_path)

    print("=" * 60)
    print("GenomeFlowAI - Dataset Preprocessing")
    print("=" * 60)

    # ==========================================================
    # Dataset Information
    # ==========================================================

    print(f"\nRows    : {df.shape[0]}")
    print(f"Columns : {df.shape[1]}")

    print("\nColumns")
    print("-" * 60)
    print(df.columns.tolist())

    print("\nData Types")
    print("-" * 60)
    print(df.dtypes)

    # ==========================================================
    # Missing Values
    # ==========================================================

    print("\nMissing Values")
    print("-" * 60)
    print(df.isnull().sum())

    # ==========================================================
    # Duplicate Rows
    # ==========================================================

    duplicate_rows = df.duplicated().sum()

    print("\nDuplicate Rows")
    print("-" * 60)
    print(duplicate_rows)

    # ==========================================================
    # Dataset Validation
    # ==========================================================

    print("\nValidation")
    print("-" * 60)

    print(
        f"Generation Range               : "
        f"{df['generation'].min()} - {df['generation'].max()}"
    )

    print(
        f"Clone Size Range               : "
        f"{df['clone_size'].min()} - {df['clone_size'].max()}"
    )

    print(
        f"Clone Fraction Range           : "
        f"{df['clone_fraction'].min():.6f} - "
        f"{df['clone_fraction'].max():.6f}"
    )

    print(
        f"Average Fitness Range          : "
        f"{df['average_fitness'].min():.6f} - "
        f"{df['average_fitness'].max():.6f}"
    )

    print(
        f"Average Age Range              : "
        f"{df['average_age'].min():.2f} - "
        f"{df['average_age'].max():.2f}"
    )

    print(
        f"Passenger Mutation Range       : "
        f"{df['average_passenger_mutations'].min():.2f} - "
        f"{df['average_passenger_mutations'].max():.2f}"
    )

    print(
        f"Driver Mutation Count Range    : "
        f"{df['driver_mutation_count'].min()} - "
        f"{df['driver_mutation_count'].max()}"
    )

    print(
        f"Aggressiveness Score Range     : "
        f"{df['aggressiveness_score'].min():.6f} - "
        f"{df['aggressiveness_score'].max():.6f}"
    )

    # ==========================================================
    # Invalid Records
    # ==========================================================

    invalid_clone_fraction = (
        (df["clone_fraction"] < 0.0)
        | (df["clone_fraction"] > 1.0)
    ).sum()

    invalid_driver_count = (
        (df["driver_mutation_count"] < 0)
        | (df["driver_mutation_count"] > 8)
    ).sum()

    invalid_clone_size = (
        df["clone_size"] <= 0
    ).sum()

    print("\nInvalid Records")
    print("-" * 60)

    print(f"Invalid Clone Fraction : {invalid_clone_fraction}")
    print(f"Invalid Driver Count   : {invalid_driver_count}")
    print(f"Invalid Clone Size     : {invalid_clone_size}")

    # ==========================================================
    # Sort Dataset
    # ==========================================================

    df = (
        df.sort_values(
            by=[
                "simulation_id",
                "generation",
                "clone"
            ]
        )
        .reset_index(drop=True)
    )

    print("\nDataset successfully validated and sorted.")

    # ==========================================================
    # Generate ML Labels
    # ==========================================================

    threshold = df["aggressiveness_score"].quantile(0.75)

    print("\nAggressiveness Threshold")
    print("-" * 60)
    print(f"75th Percentile : {threshold:.6f}")

    df["aggressive"] = (
        df["aggressiveness_score"] >= threshold
    ).astype(int)

    # ==========================================================
    # Class Distribution
    # ==========================================================

    print("\nClass Distribution")
    print("-" * 60)

    print(df["aggressive"].value_counts())

    print("\nClass Percentage")
    print("-" * 60)

    print(
        (
            df["aggressive"]
            .value_counts(normalize=True)
            * 100
        ).round(2)
    )

    # ==========================================================
    # Save Processed Dataset
    # ==========================================================

    output_path = "data/processed_dataset.csv"

    df.to_csv(
        output_path,
        index=False
    )

    print("\nProcessed dataset saved successfully.")
    print(f"Location : {output_path}")


if __name__ == "__main__":
    main()