import subprocess
import os

def main():
    # Ścieżka do programu calki.exe
    program_name = "./calki.exe"  # Plik calki.exe w tym samym folderze
    output_file = "results3.txt"  # Zapis do nowego pliku wynikowego
    num_intervals = 2000000000  # Zmieniona liczba przedziałów
    max_threads = 50  # Maksymalna liczba wątków

    # Sprawdzenie, czy plik calki.exe istnieje
    if not os.path.isfile(program_name):
        print(f"Błąd: Plik {program_name} nie został znaleziony w katalogu: {os.getcwd()}")
        return

    print(f"Plik {program_name} znaleziony. Rozpoczynam działanie.")

    # Otwórz plik wynikowy
    with open(output_file, "w") as file:
        file.write("Liczba watkow,Czas [s]\n")  # Nagłówek pliku

        for threads in range(1, max_threads + 1):
            # Budowanie komendy
            command = [program_name, str(num_intervals), str(threads)]
            print(f"Uruchamiam: {' '.join(command)}")  # Debug: Wyświetl uruchamianą komendę

            try:
                # Uruchomienie programu i przechwycenie wyjścia
                result = subprocess.run(command, capture_output=True, text=True)
                print(f"Program zwrócił kod: {result.returncode}")  # Debug: Kod zwrócony przez program

                if result.returncode != 0:
                    print(f"Błąd: Program zwrócił błąd przy wątkach = {threads}")
                    print(f"Wyjście błędu:\n{result.stderr}")
                    continue

                # Przechwycenie wyjścia programu
                output = result.stdout
                print(f"Wyjście programu dla {threads} wątków:\n{output}")  # Debug: Wyświetl wyjście programu

                # Wyciąganie czasu obliczeń z wyjścia programu
                elapsed_time = None
                for line in output.splitlines():
                    if "Czas obliczeń:" in line:
                        elapsed_time = float(line.split()[-2])  # Pobranie czasu w sekundach

                if elapsed_time is not None:
                    file.write(f"{threads},{elapsed_time}\n")  # Zapis do pliku
                    print(f"Wątki: {threads}, Czas: {elapsed_time} s")
                else:
                    print(f"Błąd: Nie znaleziono czasu obliczeń w wyjściu dla {threads} wątków.")

            except Exception as e:
                print(f"Nie udało się uruchomić komendy: {' '.join(command)}")
                print(f"Wyjątek: {e}")

    print(f"Wyniki zostały zapisane w pliku {output_file}")

if __name__ == "__main__":
    main()
