import joblib
import pandas as pd


def main():

    # ==========================================================
    # Load Model
    # ==========================================================

    model_path = "models/xgboost_model.joblib"

    model = joblib.load(model_path)

    print("=" * 60)
    print("GenomeFlowAI - Clone Aggressiveness Prediction")
    print("=" * 60)

    # ==========================================================
    # User Input
    # ==========================================================

    generation = int(input("Generation (0-100): "))

    clone_size = int(input("Clone Size: "))

    clone_fraction = float(input("Clone Fraction (0-1): "))

    average_fitness = float(input("Average Fitness: "))

    average_age = float(input("Average Age: "))

    average_passenger_mutations = float(
        input("Average Passenger Mutations: ")
    )

    driver_mutation_count = int(
        input("Driver Mutation Count (0-8): ")
    )

    # ==========================================================
    # Input Validation
    # ==========================================================

    if not (0 <= generation <= 100):
        raise ValueError("Generation must be between 0 and 100.")

    if clone_size <= 0:
        raise ValueError("Clone Size must be greater than 0.")

    if not (0.0 <= clone_fraction <= 1.0):
        raise ValueError("Clone Fraction must be between 0 and 1.")

    if average_fitness <= 0:
        raise ValueError("Average Fitness must be greater than 0.")

    if average_age < 0:
        raise ValueError("Average Age cannot be negative.")

    if average_passenger_mutations < 0:
        raise ValueError(
            "Average Passenger Mutations cannot be negative."
        )

    if not (0 <= driver_mutation_count <= 8):
        raise ValueError(
            "Driver Mutation Count must be between 0 and 8."
        )

    # ==========================================================
    # Create Feature DataFrame
    # ==========================================================

    sample = pd.DataFrame(
        [
            {
                "generation": generation,
                "clone_size": clone_size,
                "clone_fraction": clone_fraction,
                "average_fitness": average_fitness,
                "average_age": average_age,
                "average_passenger_mutations":
                    average_passenger_mutations,
                "driver_mutation_count":
                    driver_mutation_count,
            }
        ]
    )

    # ==========================================================
    # Prediction
    # ==========================================================

    prediction = model.predict(sample)[0]

    probability = model.predict_proba(sample)[0]

    confidence = probability[prediction] * 100

    # ==========================================================
    # Output
    # ==========================================================

    print("\n" + "=" * 60)
    print("Prediction")
    print("=" * 60)

    if prediction == 1:
        print("Clone Classification : AGGRESSIVE")
    else:
        print("Clone Classification : NON-AGGRESSIVE")

    print(f"Confidence           : {confidence:.2f}%")

    print("\nPrediction Probabilities")
    print("-" * 60)

    print(f"Non-Aggressive : {probability[0] * 100:.2f}%")
    print(f"Aggressive     : {probability[1] * 100:.2f}%")

    print("\nInput Summary")
    print("-" * 60)

    print(f"Generation                  : {generation}")
    print(f"Clone Size                  : {clone_size}")
    print(f"Clone Fraction              : {clone_fraction:.6f}")
    print(f"Average Fitness             : {average_fitness:.4f}")
    print(f"Average Age                 : {average_age:.2f}")
    print(
        f"Average Passenger Mutations : "
        f"{average_passenger_mutations:.2f}"
    )
    print(f"Driver Mutation Count       : {driver_mutation_count}")


if __name__ == "__main__":
    main()