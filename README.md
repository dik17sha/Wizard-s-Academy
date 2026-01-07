# Wizard's Academy: Fantasy Ecosystem Simulation

[cite_start]This project is a high-fidelity **Fantasy Ecosystem Simulation** developed using a shader-based **OpenGL** framework[cite: 16, 280]. [cite_start]It features a "Wizard's Academy" situated on a fantastical floating island, demonstrating advanced computer graphics techniques including dynamic lighting, hierarchical animation, and custom particle systems[cite: 17, 21, 33].

---

## 🌟 Key Features

### 🏰 Immersive Environment
* [cite_start]**Floating Island Setting:** The academy is positioned on a floating island within a magical realm[cite: 21, 46].
* [cite_start]**Environment Design:** Features detailed skybox management to simulate vast space and clouds below the terrain[cite: 47, 92].
* [cite_start]**Magical Sky Constraint:** To maintain a mystical aesthetic, the sky remains blue throughout the cycle[cite: 50, 165]. 
* [cite_start]**Visual Cues:** The environment reacts to time changes through shifting shadow intensity and local object luminosity rather than standard sky transitions[cite: 50, 166].

### ☀️ Dynamic Time-of-Day System
[cite_start]The simulation implements a procedural lighting cycle covering Daylight, Sunset, and Night[cite: 56, 155].
* [cite_start]**Color Interpolation:** Uses `glm::mix` to transition light color between cool night tones, warm sunset oranges, and bright daylight yellows[cite: 160, 161, 181].
* [cite_start]**Shifting Shadows:** The directional light's angle is updated in real-time based on trigonometric functions[cite: 162, 163].
* [cite_start]**Visual Communication:** Shadows rotate across the courtyard to communicate the passage of time to the user[cite: 163, 164].

### 🪄 Advanced Magical Effects
* [cite_start]**Custom Particle System:** Spellcasting is represented by a system using billboarded 2D geometry that always faces the camera[cite: 61, 64, 233].
* [cite_start]**Additive Blending:** Utilizes `glBlendFunc(GL_SRC_ALPHA, GL_ONE)` to ensure spells look like luminous light sources that retain brilliance in all lighting conditions[cite: 65, 66, 243].
* [cite_start]**Hierarchical Animation:** Particles follow complex paths combining circular orbits and vertical sinusoidal oscillations relative to the wizard's position[cite: 122, 125, 126].

---

## 🛠️ Technical Implementation

### Core Graphics Pipeline
* [cite_start]**Language & API:** C++ and Modern OpenGL Core Profile[cite: 72, 74, 83].
* [cite_start]**Shading Model:** Per-fragment **Phong Illumination** (Ambient, Diffuse, and Specular components)[cite: 199, 201].
* [cite_start]**Model Loading:** Utilizes the **Assimp** library to parse 3D models (OBJ format) and extract vertex data, normals, and UV coordinates[cite: 89, 219, 225].
* [cite_start]**Camera System:** First-person interactive camera with W/A/S/D movement and mouse-controlled pitch and yaw[cite: 98, 100].

### Dependencies
* [cite_start]**GLFW:** Window creation and input handling[cite: 73, 100].
* [cite_start]**GLAD:** OpenGL function pointer loading[cite: 73, 86].
* [cite_start]**GLM:** Matrix transformations, vector operations, and trigonometric functions[cite: 305, 306].
* [cite_start]**Assimp:** 3D model importation[cite: 89, 303].

---

## 🚀 Getting Started

### Prerequisites
* [cite_start]OpenGL 3.3+ (Core Profile)[cite: 81, 83].
* C++ Compiler.
* [cite_start]External Libraries: GLFW, GLAD, GLM, and Assimp[cite: 8].

### Installation & Usage
1. Clone the repository:
   ```bash
   git clone [https://github.com/dik17sha/Wizard-s-Academy.git](https://github.com/dik17sha/Wizard-s-Academy.git)
2. [cite_start]**Assets:** Ensure 3D models (e.g., `de_haar_castle.obj`) are located in the `Wizard Academy/assets/` directory[cite: 94].
3. [cite_start]**Build:** Compile using your preferred C++ environment, ensuring the libraries (GLFW, GLAD, GLM, Assimp) are correctly linked[cite: 73, 89, 305].
4. [cite_start]**Controls:** * **W/S:** Forward/Backward movement[cite: 108].
    * [cite_start]**A/D:** Left/Right movement[cite: 109].
    * [cite_start]**Mouse:** Pitch and Yaw rotation[cite: 100].

---

## 🎥 Demonstration

* [cite_start]**Source Code:** [GitHub Repository](https://github.com/dik17sha/Wizard-s-Academy) [cite: 299]
* [cite_start]**Video Demo:** [YouTube Demonstration Video](https://youtu.be/rMm-r9_Tylk) [cite: 323]

[cite_start]**Author:** Diksha Chottani [cite: 2]  
[cite_start]**Date:** December 6, 2025 [cite: 3]