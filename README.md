# 🧙‍♂️ Wizard’s Academy: Fantasy Ecosystem Simulation

**Wizard’s Academy** is a high-fidelity **fantasy ecosystem simulation** built using **Modern OpenGL** and C++.  
The project presents a magical academy situated on a floating island, designed to showcase advanced computer graphics techniques such as dynamic lighting, hierarchical animation, and custom particle systems.

The simulation emphasizes visual storytelling through lighting, motion, and environmental feedback rather than traditional UI elements, creating an immersive and cohesive fantasy experience.

---

## 🌟 Key Features

### 🏰 Immersive Environment
- **Floating Island Setting:** The Wizard’s Academy exists on a levitating landmass suspended above a cloud-filled void.
- **Skybox & Atmosphere:** A carefully managed skybox creates the illusion of vast space and depth beneath the island.
- **Magical Sky Constraint:** To maintain a consistent mystical aesthetic, the sky remains blue throughout the simulation.
- **Environmental Feedback:** Time progression is conveyed through changes in shadow orientation, intensity, and object illumination rather than sky color transitions.

---

### ☀️ Dynamic Time-of-Day System
The environment simulates a procedural lighting cycle consisting of **Day**, **Sunset**, and **Night**.

- **Smooth Color Transitions:** Light colors are interpolated using `glm::mix`, blending cool night tones, warm sunset hues, and bright daylight lighting.
- **Moving Light Source:** The directional light rotates dynamically using trigonometric functions to simulate the sun’s movement.
- **Shadow-Based Time Cues:** Rotating shadows across the courtyard visually communicate the passage of time.

---

### 🪄 Advanced Magical Effects
- **Custom Particle System:** Spellcasting effects are rendered using billboarded particles that always face the camera.
- **Additive Blending:** Spells use additive blending (`GL_SRC_ALPHA`, `GL_ONE`) to appear luminous and unaffected by scene lighting.
- **Hierarchical Animation:** Particles follow complex motion paths that combine circular orbits with vertical sinusoidal oscillations relative to the wizard character.

---

## 🛠️ Technical Implementation

### Core Graphics Pipeline
- **Language & API:** C++ with Modern OpenGL (Core Profile)
- **Shading Model:** Per-fragment **Phong illumination** with ambient, diffuse, and specular components
- **Model Loading:** 3D models are imported using **Assimp**, extracting vertex data, normals, and texture coordinates
- **Camera System:** First-person camera with keyboard movement and mouse-controlled pitch and yaw

---

### Dependencies
- **GLFW** – Window creation and input handling  
- **GLAD** – OpenGL function loading  
- **GLM** – Vector math, matrix transformations, and trigonometric utilities  
- **Assimp** – 3D model import and processing  

---

## 🚀 Getting Started

### Prerequisites
- OpenGL **3.3+ Core Profile**
- C++ compiler (GCC / Clang / MSVC)
- External libraries: GLFW, GLAD, GLM, Assimp

---

### Installation & Usage

1. **Clone the repository**
   ```bash
   git clone https://github.com/dik17sha/Wizard-s-Academy.git

2. **Assets**
   - Ensure all 3D models (e.g., `de_haar_castle.obj`) are placed inside:
     ```
     Wizard Academy/assets/
     ```

3. **Build**
   - Compile the project using your preferred C++ build system.
   - Make sure **GLFW**, **GLAD**, **GLM**, and **Assimp** are correctly linked.

4. **Controls**
   - **W / S** — Move forward / backward  
   - **A / D** — Strafe left / right  
   - **Mouse** — Control camera pitch and yaw  

---

## 🎥 Demonstration

- **Source Code:**  
  https://github.com/dik17sha/Wizard-s-Academy

- **Video Demo:**  
  https://youtu.be/rMm-r9_Tylk

---

## 📌 Project Information

- **Author:** Diksha Chottani  
- **Date:** December 6, 2025

