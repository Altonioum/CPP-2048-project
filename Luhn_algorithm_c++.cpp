#include <iostream>  

// VALIDATION: Ensures the input is exactly 16 digits and contains no spaces or letters 

bool isValid(const std::string& input, std::string& error) { 

    if (input.length() != 16) { 

        error = "Reject: " + std::to_string(input.length()) + " digits"; 

        return false; 

    } 

    for (char c : input) { 

        if (std::isspace(c)) { 

            error = "Reject: malformed input"; 

            return false; 

        } 

        if (!std::isdigit(c)) { 

            error = "Reject non-digit"; 

            return false; 

        } 

    } 

    return true; 

} 

//CALCULATION: Processes digits right-to-left and verifies the sum is divisible by 10 

void checkLuhn(const std::string& s) { 

    int sum = 0; 

    bool doubleDigit = false; 

     

    for (int i = 15; i >= 0; --i) { 

        int n = s[i] - '0';  

        if (doubleDigit) { 

            n *= 2; 

            if (n > 9) n -= 9; 

        } 

        sum += n; 

        doubleDigit = !doubleDigit; 

    } 

    if (sum % 10 == 0) std::cout << "Correct format; checksum passes" << std::endl; 

    else std::cout << "Correct format; checksum fails" << std::endl; 

} 

// MAIN LOOP: Handles user interaction, validating and checking in a loop 

int main() { 

    std::string input; 

    while (true) { 

        std::cout << "\nEnter 16-digit number (or 'exit'): "; 

        if (!(std::cin >> input) || input == "exit") break; 

 

        std::string error; 

        if (isValid(input, error)) checkLuhn(input); 

        else std::cout << error << std::endl; 

    } 

    std::cout << "Exiting program. Goodbye!" << std::endl; 

    return 0; 

} 