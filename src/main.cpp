#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <sstream>
#include <algorithm>
#include <set>
#include <chrono>

// Function prototypes
int calculateReward(const std::vector<std::pair<std::string, std::pair<int, int>>> &path,
                    const std::vector<std::vector<std::string>> &sequences,
                    const std::vector<int> &rewards);

void findPaths(const std::vector<std::vector<std::string>> &matrix,
               int buffer_size,
               int row,
               int col,
               std::vector<std::pair<std::string, std::pair<int, int>>> &currentPath,
               const std::vector<std::vector<std::string>> &sequences,
               const std::vector<int> &rewards,
               int &maxReward,
               bool isVerticalMove,
               std::vector<std::vector<bool>> &visited,
               std::vector<std::pair<std::string, std::pair<int, int>>> &highestRewardPath);

int main()
{
  std::cout << "\n\nWelcome to Breach Protocol!\n";
  std::cout << "Do you want to read the input from a file in the src folder? (y/n): ";
  char answer;
  std::cin >> answer;
  if (answer == 'y')
  {
    std::string filename;
    std::cout << "\nEnter the name of the file: ";
    std::cin >> filename;

    std::ifstream inputFile("../src/" + filename);
    if (inputFile)
    {
      int buffer_size, matrix_width, matrix_height, number_of_sequences;
      inputFile >> buffer_size >> matrix_width >> matrix_height;
      std::vector<std::vector<std::string>> matrix(matrix_height, std::vector<std::string>(matrix_width));
      // Read matrix
      for (int i = 0; i < matrix_height; i++)
      {
        for (int j = 0; j < matrix_width; j++)
        {
          inputFile >> matrix[i][j];
        }
      }

      inputFile >> number_of_sequences;
      std::vector<std::vector<std::string>> sequences(number_of_sequences);
      std::vector<int> rewards(number_of_sequences);
      // Read Sequences and Rewards
      std::string line;
      for (int i = 0; i < number_of_sequences; i++)
      {
        while (std::getline(inputFile, line) && line.empty()) // Consume newline char
          ;
        std::istringstream iss(line);
        std::string token;
        while (iss >> token)
        {
          sequences[i].push_back(token);
        }

        inputFile >> rewards[i];
      }

      inputFile.close();

      std::vector<std::pair<std::string, std::pair<int, int>>> currentPath;
      std::vector<std::vector<bool>> visited(matrix_height, std::vector<bool>(matrix_width, false));
      std::vector<std::pair<std::string, std::pair<int, int>>> highestRewardPath;
      int maxReward = 0;

      auto start = std::chrono::high_resolution_clock::now();
      for (int col = 0; col < matrix_width; col++)
      {
        findPaths(matrix, buffer_size, 0, col, currentPath, sequences, rewards, maxReward, true, visited, highestRewardPath);
      }
      auto stop = std::chrono::high_resolution_clock::now();

      std::cout << "\nTotal Reward: " << maxReward << "\n";
      std::cout << "Highest Reward Path: ";
      for (const auto &token : highestRewardPath)
      {
        std::cout << token.first << " ";
      }
      std::cout << "\n";
      for (const auto &token : highestRewardPath)
      {
        std::cout << token.second.second + 1 << ", " << token.second.first + 1 << "\n";
      }
      auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
      std::cout << "\nExecution time: " << duration.count() << " ms\n";
      std::cout << "Do you want to write the output to a file? (y/n): ";
      char writeToFile;
      std::cin >> writeToFile;
      if (writeToFile == 'y')
      {
        std::string filename;
        std::cout << "\nEnter the name of the file: ";
        std::cin >> filename;

        std::ofstream outputFile("../test/" + filename);
        if (outputFile)
        {
          outputFile << "Total Reward: " << maxReward << "\n";
          outputFile << "Highest Reward Path: ";
          for (const auto &token : highestRewardPath)
          {
            outputFile << token.first << " ";
          }
          outputFile << "\n";
          for (const auto &token : highestRewardPath)
          {
            outputFile << token.second.second + 1 << ", " << token.second.first + 1 << "\n";
          }
          auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
          outputFile << "\nExecution time: " << duration.count() << " ms";
          std::cout << "\nThank you for playing this minigame!\n#MenujuPerubahan";
          outputFile.close();
        }
      }
    }
    else
    {
      std::cout << "Unable to open file: " << filename << "\n";
    }
  }
  else
  {
    int buffer_size, matrix_width, matrix_height, number_of_sequences, token_size, maximal_sequence_length;
    std::cout << "\nEnter the matrix size (W x H): ";
    std::cin >> matrix_width >> matrix_height;
    std::vector<std::vector<std::string>> matrix(matrix_height, std::vector<std::string>(matrix_width));

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

    std::vector<std::vector<std::string>> sequences(number_of_sequences);
    std::vector<int> rewards(number_of_sequences);

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
    std::set<std::vector<std::string>> unique_sequences;
    for (int i = 0; i < number_of_sequences; i++)
    {
      int sequence_length = dis_seq(gen);
      std::vector<std::string> sequence;
      for (int j = 0; j < sequence_length; j++)
      {
        sequence.push_back(token[dis(gen)]);
      }

      // Check if the sequence is unique
      while (unique_sequences.count(sequence) > 0)
      {
        sequence.clear();
        for (int j = 0; j < sequence_length; j++)
        {
          sequence.push_back(token[dis(gen)]);
        }
      }

      unique_sequences.insert(sequence);

      sequences[i] = sequence;
      rewards[i] = dis_reward(gen);

      std::cout << "\nSequence " << i + 1 << ": ";
      for (const auto &t : sequences[i])
      {
        std::cout << t << " ";
      }
      std::cout << "\nReward " << i + 1 << ": " << rewards[i] << "\n";
    }

    std::vector<std::pair<std::string, std::pair<int, int>>> currentPath;
    int maxReward = 0;
    std::vector<std::vector<bool>> visited(matrix_height, std::vector<bool>(matrix_width, false));
    std::vector<std::pair<std::string, std::pair<int, int>>> highestRewardPath;

    auto start = std::chrono::high_resolution_clock::now();
    for (int col = 0; col < matrix_width; col++)
    {
      findPaths(matrix, buffer_size, 0, col, currentPath, sequences, rewards, maxReward, true, visited, highestRewardPath);
    }
    auto stop = std::chrono::high_resolution_clock::now();

    // Print the highest reward path
    std::cout << "\nTotal Reward: " << maxReward << "\n";
    std::cout << "Highest Reward Path: ";
    for (const auto &token : highestRewardPath)
    {
      std::cout << token.first << " ";
    }
    std::cout << "\n";
    for (const auto &token : highestRewardPath)
    {
      std::cout << token.second.second + 1 << ", " << token.second.first + 1 << "\n";
    }
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

    std::cout << "\nExecution time: " << duration.count() << " ms\n";
    std::cout << "Do you want to write the output to a file? (y/n): ";
    char writeToFile;
    std::cin >> writeToFile;
    if (writeToFile == 'y')
    {
      std::string filename;
      std::cout << "\nEnter the name of the file: ";
      std::cin >> filename;

      std::ofstream outputFile("../test/" + filename);
      if (outputFile)
      {
        outputFile << "Total Reward: " << maxReward << "\n";
        outputFile << "Highest Reward Path: ";
        for (const auto &token : highestRewardPath)
        {
          outputFile << token.first << " ";
        }
        outputFile << "\n";
        for (const auto &token : highestRewardPath)
        {
          outputFile << token.second.second + 1 << ", " << token.second.first + 1 << "\n";
        }
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        outputFile << "\nExecution time: " << duration.count() << " ms";
        std::cout << "\nThank you for playing this minigame!\n#MenujuPerubahan";
        outputFile.close();
      }
    }
  }

  return 0;
}

int calculateReward(const std::vector<std::pair<std::string, std::pair<int, int>>> &path, const std::vector<std::vector<std::string>> &sequences, const std::vector<int> &rewards)
{
  int totalReward = 0;

  for (int i = 0; i < sequences.size(); i++)
  {
    const std::vector<std::string> &sequence = sequences[i];
    if (sequence.size() > path.size())
    {
      continue;
    }

    for (int j = 0; j <= path.size() - sequence.size(); j++)
    {
      bool match = true;
      for (int k = 0; k < sequence.size(); k++)
      {
        if (path[j + k].first != sequence[k])
        {
          match = false;
          break;
        }
      }

      if (match)
      {
        totalReward += rewards[i];
        break;
      }
    }
  }

  return totalReward;
}

void findPaths(const std::vector<std::vector<std::string>> &matrix,
               int buffer_size,
               int row,
               int col,
               std::vector<std::pair<std::string, std::pair<int, int>>> &currentPath,
               const std::vector<std::vector<std::string>> &sequences,
               const std::vector<int> &rewards,
               int &maxReward,
               bool isVerticalMove,
               std::vector<std::vector<bool>> &visited,
               std::vector<std::pair<std::string, std::pair<int, int>>> &highestRewardPath)
{
  if (row >= matrix.size() || col < 0 || col >= matrix[0].size() || visited[row][col])
  {
    return;
  }

  visited[row][col] = true; // Mark the current position as visited

  currentPath.push_back({matrix[row][col], {row, col}});

  int currentReward = calculateReward(currentPath, sequences, rewards);
  if (currentReward > maxReward)
  {
    maxReward = currentReward;
    highestRewardPath = currentPath;
  }

  if (currentPath.size() < buffer_size)
  {
    for (int jump = 1; jump < matrix.size(); jump++)
    {
      if (isVerticalMove)
      {
        findPaths(matrix, buffer_size, row + jump, col, currentPath, sequences, rewards, maxReward, false, visited, highestRewardPath);
        findPaths(matrix, buffer_size, row - jump, col, currentPath, sequences, rewards, maxReward, false, visited, highestRewardPath);
      }
      else
      {
        findPaths(matrix, buffer_size, row, col + jump, currentPath, sequences, rewards, maxReward, true, visited, highestRewardPath);

        findPaths(matrix, buffer_size, row, col - jump, currentPath, sequences, rewards, maxReward, true, visited, highestRewardPath);
      }
    }
  }

  currentPath.pop_back();

  visited[row][col] = false;
}
