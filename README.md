# 3D Rotating Donut

A simple **3D spinning donut** rendered in ASCII characters using C and basic mathematical transformations.

## 📌 Overview
This program generates an animated 3D torus (donut) using ASCII characters and rotates it in real-time. The rendering is done using **basic trigonometric transformations** and a **z-buffer** for depth calculations.

### 🔍 How It Works
- The donut is represented as a set of points in 3D space.
- These points are rotated using **matrix transformations**.
- The final 2D projection is displayed in the terminal.
- A **z-buffer** ensures correct visibility handling.

![Mathematical Representation](https://github.com/user-attachments/assets/f6707c96-effd-4ff5-9933-7f8a18c31f29)

## 🚀 Installation & Usage

### **1️⃣ Compilation**
Ensure you have **GCC** installed, then compile the program:
```sh
gcc main.c -lm
