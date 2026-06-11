# Trenser QuickBook

## Introduction

QuickBook is a console-based movie ticket booking system designed to automate cinema booking operations through a role-driven workflow. It supports three primary users — Admin, Theatre Owner, and Customer — each with distinct responsibilities to ensure efficient task distribution.

The system emphasizes:

```
- Secure authentication and role management with password encryption

- Streamlined theatre, screen, show, seat, booking, and payment workflows

- Notifications for booking confirmations, updates, and cancellations

- Activity and error logging for system monitoring and audit trails

- Data persistence via CSV file handling
```

This project is a new product development initiative aimed at providing a simple, text-based solution for movie ticket booking without GUI or cloud dependencies.

## Getting Started

### Installation

```
- Clone the repository to your local machine.

- Ensure you have a C++ compiler (GCC/MinGW/MSVC) installed.

- Open the project in Visual Studio 2022 or any IDE supporting C++11 or later.

- Ensure the Files/ directory (containing all CSV data files) is present in the working directory.
```

## Software Dependencies

```
- Language: C++

- Concepts Used: OOP, inheritance, pointers, file handling, serialization, singleton pattern, service layer, factory pattern

- No external frameworks or libraries required
```

## Supported Platforms

```
- Windows 10/11

- Linux distributions with standard C++ compilation support
```

## Data Storage

```
- All user, theatre, movie, show, booking, payment, ticket, refund, notification, and log data
  is stored in CSV files inside the Files/ directory using file-handling utilities
  (FileManagement, Datalinker). Data is loaded at startup and saved on exit.
```

## Build and Test

### Build

```
- Compile using GCC/MinGW/MSVC with C++11 or later.

- Ensure all .h and .cpp files across Controller/, DataStore/, Factories/, Models/,
  Services/, Utilities/, and Views/ are compiled together.

- The Files/ directory must be present in the working directory at runtime.
```

### Test

```
- Validate each service module independently (authentication, booking,
  payment, refund, notification, logging).

- Integration Testing: Ensure smooth data flow between modules
  (e.g., seat selection → booking → payment → ticket → notification).

- System Testing: End-to-end scenarios such as:
    Customer registration → login → browse shows → select seats → pay → receive ticket → cancel → refund
    Theatre Owner → add theatre → add screen → assign movie → schedule show → view bookings
    Admin → approve theatre → add movie → manage users → view system logs

- Acceptance Testing: Verify against defined acceptance criteria for each user role's dashboard and workflows.

- Error Handling: Test invalid inputs (e.g., wrong email format, weak password, duplicate phone number,
  invalid card/UPI details, show time conflicts, ineligible cancellations).
```

## Contribute

We welcome contributions to improve QuickBook! Ways to contribute:

```
- Bug Fixes: Identify and resolve issues in existing modules.

- Feature Enhancements: Suggest or implement new functionalities.

- Documentation: Improve clarity in SRS, scope, or README.

- Testing: Add more unit/integration test cases.
```

To contribute:

```
- Fork the repository.

- Create a feature branch.

- Commit changes with clear messages.

- Submit a pull request.
```