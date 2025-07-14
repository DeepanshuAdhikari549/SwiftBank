Swift Bank - C Based Banking Management System

**Swift Bank** is a console-based banking application built in C. It allows both admins and users to securely manage customer accounts, perform transactions, and explore banking services. The system is designed to demonstrate core data structures, file handling, and algorithm implementation in a real-world-like project.

## 🔧 Project Overview

Swift Bank provides a fully functional banking experience through a terminal interface. The system supports:
- User and Admin authentication
- Account creation, editing, deletion
- Deposits, withdrawals, balance inquiry
- Money transfer between accounts
- Transaction history and branch location features
- Persistent storage via `.txt` files

## 🚀 Features

### 🔐 User Authentication
- Role-based login for **Admin** and **User**
- Secure password input (hidden with `*`)
- Hardcoded credentials for testing:
  - **Admin:** `admin` / `shadow`
  - **User:** `user` / `byte`

### 👨‍💼 Admin Panel
- Add, edit, delete customer accounts
- View customer list
- Search by name or account number
- Process and view all transactions
- Find nearest bank branch using Dijkstra’s Algorithm
- About Us section (Team details)
- 
### 🧑 User Panel
- Check account balance
- Deposit and withdraw money
- Transfer money to other accounts
- View transaction statement

---

### 🎨 Console UI
- Centered text display
- Colored menus and banners
- Loading screens and user-friendly prompts

## ⚙️ Technologies Used

- **Language:** C
- **Libraries:** `stdio.h`, `stdlib.h`, `string.h`, `conio.h`, `windows.h`, `time.h`
- **Data Structures:**
  - Linked List – For dynamic customer record storage
  - Stack – For recent transactions
  - Queue – For pending transactions
  - Graph – For nearest branch feature
- **File Handling:** `.txt` files to store accounts and transaction data

## 🛠️ How to Run

### ✅ Requirements
- Windows OS (due to `windows.h`)
- GCC or Turbo C++ Compiler

### ▶️ Steps
```bash
1. Open terminal or CMD in project folder
2. Compile the code:
   gcc SwiftBank.c -o SwiftBank.exe
3. Run the program:
   ./SwiftBank.exe


## 👨‍💻 Authors

Developed under the guidance of **Mr.Mohit Chauhan**

**Team SHADOW BYTE**
- 🧑‍💼 Rohit Pant – 24711135 – rohitpant2815@gmail.com  
- 👨‍💻 Deepanshu Adhikari – 24711267 – deepanshuadhikari549@gmail.com  
- 🎨 Anamika – 24712410 – anupundir@gmail.com  
- 🧪 Anisha Kumari – 24392165 – anishasundriya@gmail.com  
