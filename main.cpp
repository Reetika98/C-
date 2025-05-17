/*
 * Terminal Quiz Game in C++
 * 
 * Features:
 * - Loads multiple-choice questions from a JSON file (questions.json).
 * - Tracks score, attempts, and time per question.
 * - Shows a summary screen at the end.
 * 
 * Uses nlohmann::json library (single header) for JSON parsing.
 * 
 * Build:
 *   g++ -std=c++17 -o quiz main.cpp
 * 
 * Run:
 *   ./quiz
 * 
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <limits>   // for numeric_limits
#include <iomanip>  // for formatting
#include <algorithm> // for transform

// Include the nlohmann::json single header
// You need to download json.hpp from https://github.com/nlohmann/json and place it in the project directory
#include "json.hpp"

using json = nlohmann::json;
using namespace std;
using namespace std::chrono;

// Struct to hold a single question
struct Question {
    string text;
    vector<string> choices;
    int answer_index; // zero based index of correct answer
};

// Function to load questions from a JSON file
bool load_questions(const string& filename, vector<Question>& questions) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open questions file: " << filename << endl;
        return false;
    }

    json j;
    try {
        file >> j;
        if (!j.is_array()) {
            cerr << "Error: Invalid questions format. Expected a JSON array." << endl;
            return false;
        }

        for (const auto& item : j) {
            if (!item.contains("question") || !item.contains("choices") || !item.contains("answer")) {
                cerr << "Warning: A question is missing required fields. Skipping." << endl;
                continue;
            }
            Question q;
            q.text = item["question"].get<string>();
            q.choices = item["choices"].get<vector<string>>();
            q.answer_index = item["answer"].get<int>();

            // Basic validation
            if (q.answer_index < 0 || q.answer_index >= (int)q.choices.size()) {
                cerr << "Warning: Question has invalid answer index. Skipping." << endl;
                continue;
            }
            questions.push_back(q);
        }
    } catch (const json::exception& e) {
        cerr << "Error parsing JSON: " << e.what() << endl;
        return false;
    }

    return true;
}

// Utility function to get user input choice for a question
int get_user_choice(int num_choices) {
    int choice = -1;
    while (true) {
        cout << "Your answer (enter option number 1-" << num_choices << "): ";
        if (!(cin >> choice)) {
            cout << "Invalid input, please enter a number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        if (choice < 1 || choice > num_choices) {
            cout << "Please enter a number between 1 and " << num_choices << "." << endl;
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        break;
    }
    return choice - 1; // zero-based index
}

int main() {
    cout << "=== Welcome to the Terminal Quiz Game ===" << endl;

    vector<Question> questions;
    const string question_file = "questions.json";

    if (!load_questions(question_file, questions)) {
        cerr << "Failed to load questions. Exiting." << endl;
        return 1;
    }

    if (questions.empty()) {
        cerr << "No valid questions loaded. Exiting." << endl;
        return 1;
    }

    int total_attempts = 0;
    int score = 0;
    vector<double> time_per_question;

    for (size_t i = 0; i < questions.size(); ++i) {
        const Question& q = questions[i];
        cout << "\nQuestion " << (i + 1) << " / " << questions.size() << endl;
        cout << q.text << endl;
        for (size_t idx = 0; idx < q.choices.size(); ++idx) {
            cout << idx + 1 << ". " << q.choices[idx] << endl;
        }

        // Start timer
        auto start_time = high_resolution_clock::now();

        int user_choice = get_user_choice((int)q.choices.size());

        // Stop timer
        auto end_time = high_resolution_clock::now();
        duration<double> elapsed_seconds = end_time - start_time;
        time_per_question.push_back(elapsed_seconds.count());

        total_attempts++;

        if (user_choice == q.answer_index) {
            cout << "Correct!" << endl;
            score++;
        } else {
            cout << "Wrong. Correct answer was: " << q.choices[q.answer_index] << endl;
        }
    }

    // Summary Screen
    cout << "\n=== Quiz Summary ===" << endl;
    cout << "Total Questions: " << questions.size() << endl;
    cout << "Attempts: " << total_attempts << endl;
    cout << "Score: " << score << endl;

    // total time and average time
    double total_time = 0;
    for (double t : time_per_question) {
        total_time += t;
    }
    double avg_time = (total_attempts > 0) ? total_time / total_attempts : 0.0;

    cout << fixed << setprecision(2);
    cout << "Total Time Taken: " << total_time << " seconds" << endl;
    cout << "Average Time per Question: " << avg_time << " seconds" << endl;

    cout << "Thank you for playing!" << endl;

    return 0;
}
