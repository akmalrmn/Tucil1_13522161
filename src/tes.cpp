#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <sstream>
#include <algorithm>
#include <set>

// Function prototypes
int calculateReward(const std::vector<std::string> &path,
                    const std::vector<std::vector<std::string>> &sequences,
                    const std::vector<int> &rewards);

void findPaths(const std::vector<std::vector<std::string>> &matrix,
               int buffer_size,
               int row,
               int col,
               std::vector<std::string> &currentPath,
               std::vector<std::vector<std::string>> &allPaths,
               const std::vector<std::vector<std::string>> &sequences,
               const std::vector<int> &rewards,
               int &maxReward,
               bool isVerticalMove,
               std::vector<std::vector<bool>> &visited);

void printPaths(const std::vector<std::vector<std::string>> &allPaths,
                const std::vector<std::vector<std::string>> &sequences,
                const std::vector<int> &rewards);

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

      // Consume the endline character after reading matrix_height
      inputFile.ignore();

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
      std::vector<std::vector<std::string>> sequences(number_of_sequences);
      std::vector<int> rewards(number_of_sequences);

      // Consume the endline character after reading number_of_sequences
      inputFile.ignore();

      std::string line;
      for (int i = 0; i < number_of_sequences; i++)
      {
        while (std::getline(inputFile, line) && line.empty())
          ;
        std::istringstream iss(line);
        std::string token;
        while (iss >> token)
        {
          sequences[i].push_back(token);
        }

        inputFile >> rewards[i];
        std::cout << "Sequence " << i + 1 << ": ";
        for (const auto &token : sequences[i])
        {
          std::cout << token << " ";
        }
        std::cout << "\n";
        std::cout << "Reward " << i + 1 << ": " << rewards[i] << "\n";
      }

      inputFile.close();

      std::vector<std::vector<std::string>> allPaths;
      std::vector<int> pathRewards;

      for (int col = 0; col < matrix_width; col++)
      {
        // Initialize current path for each starting column
        std::vector<std::string> currentPath;
        int maxReward = 0;
        std::vector<std::vector<bool>> visited(matrix_height, std::vector<bool>(matrix_width, false));

        findPaths(matrix, buffer_size, 0, col, currentPath, allPaths, sequences, rewards, maxReward, true, visited);

        pathRewards.push_back(maxReward);
      }

      // Print all possible paths and their rewards
      printPaths(allPaths, sequences, rewards);

      // Find and print the highest reward path
      int maxReward = INT_MIN;
      std::vector<std::string> highestRewardPath;

      for (const auto &path : allPaths)
      {
        int reward = calculateReward(path, sequences, rewards);
        if (reward > maxReward)
        {
          maxReward = reward;
          highestRewardPath = path;
        }
      }

      // Print the highest reward path
      std::cout << "Highest Reward Path: ";
      for (const auto &token : highestRewardPath)
      {
        std::cout << token << " ";
      }
      std::cout << "Total Reward: " << maxReward << "\n";
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
        // If the sequence is not unique, generate a new one
        sequence.clear();
        for (int j = 0; j < sequence_length; j++)
        {
          sequence.push_back(token[dis(gen)]);
        }
      }

      // Add the unique sequence to the set of unique sequences
      unique_sequences.insert(sequence);

      // Use the unique sequence
      sequences[i] = sequence;
      rewards[i] = dis_reward(gen);

      std::cout << "\nSequence " << i + 1 << ": ";
      for (const auto &t : sequences[i])
      {
        std::cout << t << " ";
      }
      std::cout << "\nReward " << i + 1 << ": " << rewards[i] << "\n";
    }

    std::vector<std::vector<std::string>> allPaths;
    std::vector<int> pathRewards;

    for (int col = 0; col < matrix_width; col++)
    {
      // Initialize current path for each starting column
      std::vector<std::string> currentPath;
      int maxReward = 0;
      std::vector<std::vector<bool>> visited(matrix_height, std::vector<bool>(matrix_width, false));

      findPaths(matrix, buffer_size, 0, col, currentPath, allPaths, sequences, rewards, maxReward, true, visited);

      pathRewards.push_back(maxReward);
    }

    // Print all possible paths and their rewards
    printPaths(allPaths, sequences, rewards);

    // Find and print the highest reward path
    int maxReward = INT_MIN;
    std::vector<std::string> highestRewardPath;

    for (const auto &path : allPaths)
    {
      int reward = calculateReward(path, sequences, rewards);
      if (reward > maxReward)
      {
        maxReward = reward;
        highestRewardPath = path;
      }
    }

    // Print the highest reward path
    std::cout << "Highest Reward Path: ";
    for (const auto &token : highestRewardPath)
    {
      std::cout << token << " ";
    }
    std::cout << "Total Reward: " << maxReward << "\n";

    auto maxRewardIter = std::max_element(pathRewards.begin(), pathRewards.end());
    int maxRewardIndex = std::distance(pathRewards.begin(), maxRewardIter);

    std::cout << "\nHighest Reward Path: ";
    for (const auto &token : allPaths[maxRewardIndex])
    {
      std::cout << token << " ";
    }
    std::cout << "Total Reward: " << *maxRewardIter << "\n";
  }

  return 0;
}

int calculateReward(const std::vector<std::string> &path, const std::vector<std::vector<std::string>> &sequences, const std::vector<int> &rewards)
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
        if (path[j + k] != sequence[k])
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
               std::vector<std::string> &currentPath,
               std::vector<std::vector<std::string>> &allPaths,
               const std::vector<std::vector<std::string>> &sequences,
               const std::vector<int> &rewards,
               int &maxReward,
               bool isVerticalMove,
               std::vector<std::vector<bool>> &visited) // Added visited parameter
{
  if (row >= matrix.size() || col < 0 || col >= matrix[0].size() || visited[row][col])
  {
    // Out of bounds or already visited, return
    return;
  }

  visited[row][col] = true; // Mark the current position as visited

  currentPath.push_back(matrix[row][col]);

  int currentReward = calculateReward(currentPath, sequences, rewards);
  if (currentReward > maxReward)
  {
    maxReward = currentReward;
  }

  // If the current path length is less than or equal to the buffer size, add it to allPaths
  if (currentPath.size() <= buffer_size)
  {
    allPaths.push_back(currentPath);
  }

  // If the current path length is less than the buffer size, continue exploring
  if (currentPath.size() < buffer_size)
  {
    if (isVerticalMove)
    {
      // Move to the next row (vertical move)
      findPaths(matrix, buffer_size, row + 1, col, currentPath, allPaths, sequences, rewards, maxReward, false, visited);
      findPaths(matrix, buffer_size, row - 1, col, currentPath, allPaths, sequences, rewards, maxReward, false, visited);
    }
    else
    {
      // Move to the right (horizontal move)
      findPaths(matrix, buffer_size, row, col + 1, currentPath, allPaths, sequences, rewards, maxReward, true, visited);

      // Move to the left (horizontal move)
      findPaths(matrix, buffer_size, row, col - 1, currentPath, allPaths, sequences, rewards, maxReward, true, visited);
    }
  }

  // Remove the last element to backtrack
  currentPath.pop_back();

  // Mark the current position as unvisited to allow backtracking
  visited[row][col] = false;
}

void printPaths(const std::vector<std::vector<std::string>> &allPaths,
                const std::vector<std::vector<std::string>> &sequences,
                const std::vector<int> &rewards)
{
  for (size_t i = 0; i < allPaths.size(); i++)
  {
    std::cout << "Path " << i + 1 << ": ";
    for (const auto &token : allPaths[i])
    {
      std::cout << token << " ";
    }
    int reward = calculateReward(allPaths[i], sequences, rewards);
    std::cout << "Total Reward: " << reward << "\n";
  }
}