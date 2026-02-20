#include <iostream>
#include <string>
#include "json_value.h"
#include "tokenizer.h"
#include "parser.h"

int main()
{
    std::string json_input = R"({"name": "John", "age": 30, "isStudent": false, "scores": [85.5, 90.0, 78.0], "address": {"street": "123 Main St", "city": "Anytown"}})";
    
    try {
        Tokenizer tokenizer(json_input);
        auto tokens = tokenizer.tokenize();

        Parser parser(tokens);
        JsonValue json_value = parser.parse();

        std::cout << "Parsed JSON successfully!" << std::endl;
        std::cout << "JSON Value: " << std::endl;
        json_value.print();
        std::cout << std::endl;

        std::cout << "Dumped JSON string: " << std::endl;
        std::string dumped_json = json_value.dump(2);
        std::cout << dumped_json << std::endl;

        std::string name = json_value["name"].as_string();
        std::string city = json_value["address"]["city"].as_string();
        int age = json_value["age"].as_int();
        double first_score = json_value["scores"][0].as_double();
        bool is_student = json_value["isStudent"].as_boolean();
        std::cout << "Name: " << name << std::endl;
        std::cout << "Age: " << age << std::endl;
        std::cout << "First Score: " << first_score << std::endl;
        std::cout << "Is Student: " << (is_student ? "true" : "false") << std::endl;
        std::cout << "City: " << city << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    std::cout << "Press Enter to exit...";
    std::cin.get();
    return 0;
}