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
    cd webdev-server
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


