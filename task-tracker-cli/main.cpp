#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include "json.hpp"  // Include the nlohmann JSON library

using json = nlohmann::json;
using namespace std;

struct Task {
    int id;
    string description;
    string status;
    string createdAt;
    string updatedAt;

    // Convert Task struct to JSON
    json toJson() const{
        return json{
            {"id", id},
            {"description", description},
            {"status", status},
            {"createdAt", createdAt},
            {"updatedAt", updatedAt}
        };
    }

    // Create Task struct from JSON
    static Task fromJson(const json& j) {
        return Task{
            j["id"],
            j["description"],
            j["status"],
            j["createdAt"],
            j["updatedAt"]
        };
    }
};

// UPDATE THE TASK
void updateTask(vector<Task>& tasks, int taskId, const string& newDescription) {
    for (auto& task : tasks) {
        if (task.id == taskId) {
            task.description = newDescription;
            time_t now = time(0);  // Get current time
            task.updatedAt = ctime(&now);  // Convert time to string and store in updatedAt
            cout << "Task updated successfully.\n";
            return;
        }
    }
    cout << "Task not found.\n";
}

// MARK TASK DONE OR IN PROGRESS
void markTaskStatus(vector<Task>& tasks, int taskId, const string& status) {
    for (auto& task : tasks) {
        if (task.id == taskId) {
            task.status = status;
            time_t now = time(0);  // Get current time
            task.updatedAt = ctime(&now);  // Convert time to string and store in updatedA
            cout << "Task marked as " << status << ".\n";
            return;
        }
    }
    cout << "Task not found.\n";
}


// LOAD THE TASK
vector<Task> loadTasks(const string& filename) {
    ifstream inputFile(filename);
    vector<Task> tasks;

    if (inputFile.is_open()) {
        json taskJson;
        inputFile >> taskJson;
        for (const auto& j : taskJson) {
            tasks.push_back(Task::fromJson(j));
        }
        inputFile.close();
    }

    return tasks;
}

// SAVE THE TASK
void saveTasks(const vector<Task>& tasks, const string& filename){
    ofstream outputFile(filename);
    json taskJson = json::array();

    for(const auto& Task : tasks){
        taskJson.push_back(Task.toJson());
    }

    outputFile << taskJson.dump(4);
    outputFile.close();
}

// ADD TASK

void addTask(vector <Task>& tasks, const string& description){
    int id = tasks.empty() ? 1 : tasks.back().id + 1;
    time_t now = time(0);
    string timestamp = ctime(&now);

    Task newTask{id , description, "todo", timestamp, timestamp};
    tasks.push_back(newTask);
    cout<<"Task added Successfully (ID: "<<id<<")\n";
}



int main(int argc, char* argv[]){
    const string filename = "tasks.json";
    vector<Task> tasks = loadTasks(filename);
    
    if (argc < 2) {
        cerr << "Error: No command provided.\n";
        return 1;
    }

    string command = argv[1];

    if (command == "add" && argc >= 3) {
        string description = argv[2];
        addTask(tasks, description);
        saveTasks(tasks, filename);
    } else if (command == "list") {
        for (const auto& task : tasks) {
            cout << task.id << ": " << task.description << " [" << task.status
                 << "] (Created: " << task.createdAt
                 << ", Updated: " << task.updatedAt << ")\n";
        }
    }
    // More commands for update, delete, mark-done, etc.

    return 0;

}

