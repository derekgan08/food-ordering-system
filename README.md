# CPT111 Assignment 2: Food Ordering System

## Problem Statements
We live in challenging yet interesting times. With COVID-19, social distancing becoming a solid norm and eating in a crowded restaurant is no longer possible. However, we can still enjoy the delicious food from the restaurants by having Food Ordering system.

To maintain business, restaurant needs this system to showcase their menu by keeping track the prices, delivery time, number of orders by customers and sales for each item in the menu. The manager will be able to view the total sales, the most popular dish in the menu and number of customers for the day.

The pandemic will turn you the system developer into warriors of the modern world.

## Project Overview
The Food Ordering System is a terminal-based application built using C++. It allows both restaurant managers and customers to interact with the system to place and manage orders. The system handles menu management, order processing, price updates, and sales tracking. It also calculates total payments and estimated delivery times, providing a complete solution for a restaurant's ordering and delivery needs. The system also supports customer payments and offers an intuitive user interface.

## Key Features
Food Ordering and Delivering system has 2 users, Restaurant Manager and Customer with the following basic process:
- **Restaurant Manager(s)**
    - Create/update menu
    - Update prices
    - Accept orders
    - Calculate total payments per order
    - Calculate estimated delivery time
    - Calculate total sales for a day
- **Customer(s)**
    - Order online
    - Make payments

## Technologies Used
- **C++** - The main programming language for implementing the system
- **Text Files** - For input and output file operations (menu data and sales)
- **Two-Dimensional Arrays** - For storing and processing the menu items and sales data

## How It Works
- **Restaurant Manager**:
    - Manages the menu and prices through simple text inputs.
    - Accepts customer orders and processes payments.
    - Tracks sales and generates reports on total payments and popular dishes.
- **Customer:**
    - Customers can view the menu, select items, and make payments.
    - The system calculates the estimated delivery time for each order.

## Input/Output Format
- Input is taken via terminal prompts, and the menu and prices are loaded from a text file.
- The output is displayed on the terminal screen, and updated data is stored back into text files.