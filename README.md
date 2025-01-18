# CServe - A Lightweight HTTP Server for Static and Advanced Content

## Project Overview

**WebDev Server** is a lightweight HTTP server built in C that serves static files and handles advanced features like project showcases, event listings, and recommendations. It supports multiple client connections using threads and demonstrates core networking and HTTP handling concepts. This project can serve as a base for building a dynamic web application with features like project portfolios, event management, and more.

## Features

- **Static File Serving**: Serve HTML, CSS, JS, image files, and more from a designated static directory.
- **Advanced Content Handling**:
  - **Project Showcase**: Display a list of featured projects.
  - **Event Listings**: Show upcoming events.
  - **Recommendations**: Present recommended projects.
- **Request Handling**: Support for `GET` and `POST` methods with simple routing.
- **Concurrency**: Handle multiple client connections concurrently using threads.
- **Custom HTTP Responses**: Respond with appropriate HTTP status codes and MIME types.

## Table of Contents

- [Installation](#installation)
- [Usage](#usage)
- [Project Structure](#project-structure)
- [Features and Functions](#features-and-functions)
- [Future Improvements](#future-improvements)
- [Contributing](#contributing)
- [License](#license)

## Installation

### Prerequisites

Before running the server, ensure you have the following installed:
- **C Compiler**: GCC or MSVC for compiling the C code.
- **Windows OS**: The server is built using Windows-specific libraries (`winsock2`).

### Steps to Compile and Run

1. Clone the repository to your local machine:
    ```bash
    git clone https://github.com/mdayanabbas/CServe.git
    cd CServe
    ```

2. Compile the code:
    ```bash
    gcc -o server.exe webdev.c -lws2_32
    ```

3. Run the server:
    ```bash
    ./server.exe
    ```

4. Open a browser and visit `http://localhost:8080` to interact with the server.

## Usage

Once the server is up and running, you can:

- Access static files by placing them in the `public` directory.
- Visit the following endpoints:
  - `/recommendations`: Displays recommended projects.
  - `/showcase`: Displays a list of featured projects.
  - `/events`: Shows upcoming events.
  
To stop the server, press `Ctrl+C`.

## Features and Functions

### Static File Serving
The server supports serving static files (HTML, CSS, JS, images) from the `public` directory. It determines the MIME type based on the file extension and serves it accordingly.

### Advanced Content Handling
The server handles several advanced features that could be extended:

- **Project Showcase**: Showcases a list of featured projects.
- **Event Listings**: Displays upcoming events in the community or platform.
- **Recommendations**: Sends project recommendations, which could be extended to integrate with recommendation algorithms.

### HTTP Request Handling

- **GET**: The server can handle `GET` requests and respond with static content or dynamic pages.
- **POST**: POST requests are currently not implemented for advanced features, but the server can be extended to accept form submissions.

### Thread Handling
The server handles incoming connections concurrently using threads (`CreateThread`), ensuring that multiple clients can interact with the server at the same time.

### Log Requests
Every incoming request is logged with details about the method (`GET`, `POST`) and the requested path.

### HTTP Status Codes
- **200 OK**: Successfully served content.
- **404 Not Found**: File or route not found.
- **405 Method Not Allowed**: Unsupported HTTP method.

## Future Improvements
While this project serves as a basic web server, there are several opportunities for enhancement:

- **Database Integration**: Store projects, events, and recommendations in a database (e.g., MySQL, SQLite).
- **User Authentication**: Allow users to create accounts, log in, and interact with the server.
- **Dynamic Content**: Implement POST requests for user submissions and dynamic content.
- **Security Improvements**: Add HTTPS support and sanitize inputs to prevent injection attacks.
- **Advanced Routing**: Add more dynamic routes, such as API endpoints for real-time data.

## Contributing
If you'd like to contribute to this project, feel free to fork the repository, make changes, and submit a pull request. Contributions are always welcome!

### How to Contribute
1. Fork the repository.
2. Create a new branch for your changes (`git checkout -b feature-name`).
3. Make your changes and commit them (`git commit -am 'Add new feature'`).
4. Push to the branch (`git push origin feature-name`).
5. Open a pull request.

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.
