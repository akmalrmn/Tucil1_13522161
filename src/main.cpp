#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>

int main()
{
    std::cout << "Welcome to Breach Protocol!\n";
    std::cout << "Do you want to read the input from a file? (y/n): ";
    char answer;
    std::cin >> answer;
    if (answer == 'y')
    {
        std::string filename;
        std::cout << "Enter the name of the file: ";
        std::cin >> filename;

        std::ifstream inputFile(filename);
        if (inputFile)
        {
            int buffer_size, matrix_width, matrix_height, number_of_sequences;
            inputFile >> buffer_size >> matrix_width >> matrix_height;
            std::vector<std::vector<std::string>> matrix(matrix_height, std::vector<std::string>(matrix_width));
            for (int i = 0; i < matrix_height; i++)
            {
                for (int j = 0; j < matrix_width; j++)
                {
                    inputFile >> matrix[i][j];
                    std::cout << matrix[i][j] << " ";
                }
                std::cout << "\n";
            }

            inputFile >> number_of_sequences;
            std::vector<std::string> sequences(number_of_sequences);
            std::vector<int> rewards(number_of_sequences);
            std::string line;
            for (int i = 0; i < number_of_sequences; i++)
            {
                while (std::getline(inputFile, line) && line.empty())
                    ;
                sequences[i] = line;

                inputFile >> rewards[i];
                std::cout << "Sequence " << i + 1 << ": " << sequences[i] << "\n";
                std::cout << "Reward " << i + 1 << ": " << rewards[i] << "\n";
            }

            inputFile.close();
        }
        else
        {
            std::cout << "Unable to open file: " << filename << "\n";
        }
        std::cout << "The input has been read from the file.\n";
    }

    else
    {
        int buffer_size, matrix_width, matrix_height, number_of_sequences, token_size, maximal_sequence_length;
        std::cout << "Enter the matrix size (W x H): ";
        std::cin >> matrix_width >> matrix_height;
        std::cout << "Enter the buffer size: ";
        std::cin >> buffer_size;
        std::cout << "Enter the token size: ";
        std::cin >> token_size;
        std::vector<std::string> token(token_size);
        std::cout << "Enter the token: ";
        for (int i = 0; i < token_size; i++)
        {
            std::cin >> token[i];
        }
        std::cout << "Enter number of sequences: ";
        std::cin >> number_of_sequences;
        std::cout << "Enter the maximal sequence length: ";
        std::cin >> maximal_sequence_length;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, token_size - 1);
        std::uniform_int_distribution<> dis_seq(2, maximal_sequence_length);
        std::uniform_int_distribution<> dis_reward(1, 100);

        std::vector<std::vector<std::string>> matrix(matrix_height, std::vector<std::string>(matrix_width));
        std::cout << "\nThe matrix: \n";
        for (int i = 0; i < matrix_height; i++)
        {
            for (int j = 0; j < matrix_width; j++)
            {
                matrix[i][j] = token[dis(gen)];
                std::cout << matrix[i][j] << " ";
            }
            std::cout << "\n";
        }

        std::vector<std::vector<std::string>> sequences(number_of_sequences);
        std::vector<int> rewards(number_of_sequences);
        for (int i = 0; i < number_of_sequences; i++)
        {
            int sequence_length = dis_seq(gen);
            for (int j = 0; j < sequence_length; j++)
            {
                sequences[i].push_back(token[dis(gen)]);
            }
            rewards[i] = dis_reward(gen);
            std::cout << "\nSequence " << i + 1 << ": ";
            for (const auto &t : sequences[i])
            {
                std::cout << t << " ";
            }
            std::cout << "\nReward " << i + 1 << ": " << rewards[i] << "\n";
        }
    }

    return 0;
}