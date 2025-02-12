# 🚀 Pebble Developer Tools in a Containerized Environment  

This repository provides a **containerized** development setup for [Pebble](https://rebble.io/), enabling an isolated and portable environment for Pebble SDK development. It supports both **Docker Compose** and **VS Code Dev Containers**, making it easy to get started with minimal setup.  

Happy coding! 🚀🎉  

---
Got it! Here's a refined version with clearer wording:  

---

### 🖥️ Emulator Notes  

To run the Pebble emulator inside the container, ensure your system meets the necessary display requirements based on your OS **before** running the container:  

#### **Windows Users**  
- You have **two options**:  
  1. **Use WSL2**, which includes native X11 forwarding when running GUI applications.  
  2. **Alternatively**, use an X server like [VcXsrv](https://sourceforge.net/projects/vcxsrv/) or [Xming](https://sourceforge.net/projects/xming/) if running Docker natively on Windows (outside WSL2).  

#### **Linux Users (X11 and Wayland)**  
- **X11 Users:** The container should automatically detect and use your X server if `DISPLAY` is set.  
- **Wayland Users:** Ensure your session exports:  
  ```sh
  export XDG_RUNTIME_DIR=/run/user/$(id -u)
  export WAYLAND_DISPLAY=wayland-0
  ```
#### **Mac Users (XQuartz Required)**  
- Install and launch **XQuartz**:  
  ```sh
  brew install --cask xquartz
  ```
- Ensure `DISPLAY` is correctly set inside the container:  
  ```sh
  export DISPLAY=host.docker.internal:0
  ```
- Restart your session or log out and back in after installing XQuartz.  


## ✅ Prerequisites  

Before setting up the environment, ensure you have the following installed:  

- [Docker](https://www.docker.com/) – Required to run containers  
- [VS Code](https://code.visualstudio.com/) & [Dev Containers Extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers) (Optional) – For using Dev Containers  
- [Git](https://git-scm.com/) – To clone the repository  

---

## 🚀 1. Setting Up with Docker Compose (Recommended)  

This method provides the easiest and most streamlined way to run the Pebble development environment.  

### **Steps:**  

1. **Clone the repository:**  
   ```sh
   git clone https://github.com/FBarrCa/pebble-devcontainer.git
   cd pebble-dev-container
   ```

2. **Start the container:**  
   ```sh
   docker compose up -d
   ```

3. **Access the container shell:**  
   ```sh
    docker exec -it pebble-sdk-container zsh
   ```

4. **Stop and remove the container when done:**  
   ```sh
   docker compose down
   ```

---

## 🖥️ 2. Setting Up with VS Code Dev Containers  

If you're using **VS Code**, you can leverage **Dev Containers** to automatically set up and launch the environment inside a container.  

### **Steps:**  

1. **Clone the repository:**  
   ```sh
   git clone https://github.com/FBarrCa/pebble-devcontainer.git
   cd pebble-dev-container
   ```

2. **Open VS Code and load the project folder.**  

3. When prompted, **click "Reopen in Container"**.  

4. Wait for the container to build and start—then you're ready to code!  

---


## 🤝 Contributing  

We welcome contributions! To contribute:  

1. **Fork** the repository.  
2. Create a **new branch** for your changes.  
3. Make your improvements.  
4. Submit a **Pull Request** for review.  

