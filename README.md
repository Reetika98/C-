# C-
# Terminal Quiz Game in C++

## Overview

This is a terminal-based quiz game built using C++. The quiz loads multiple-choice questions from a JSON file and tracks your score, number of attempts, and time taken per question. At the end of the quiz, a summary screen presents your results.

## Features

- Loads questions from a JSON file (`questions.json`).
- Supports multiple-choice questions.
- Tracks:
  - Score (number of correct answers)
  - Number of attempts (questions answered)
  - Time taken per question (in seconds)
- Displays a summary screen with total score, attempts, and average time per question.

## Code Structure

- `main.cpp` - The main source file containing the quiz game logic.
- `questions.json` - JSON file containing the quiz questions, choices, and answers.
- `README.md` - This file containing build and run instructions and explanation.

The program uses the [nlohmann/json](https://github.com/nlohmann/json) library (included as header only in `main.cpp`) to parse JSON.

## Building and Running

### Prerequisites

- A C++17 compatible compiler (tested with g++ and clang++).
- Make is optional but recommended for easy build.

### Build

To build the program, run:

```bash
g++ -std=c++17 -o quiz main.cpp
