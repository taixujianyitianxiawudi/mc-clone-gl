#include "voxel_engine.hpp"

VoxelEngine::VoxelEngine() {
    // Initialize window
    window = new sf::Window(sf::VideoMode(WIN_RES.x,WIN_RES.y), "Voxel Engine", sf::Style::Default, sf::ContextSettings(24));
    window->setActive(true);
    glewInit();
    // Initialize OpenGL settings
    sf::ContextSettings settings;
    settings.majorVersion = 3;
    settings.minorVersion = 3;
    settings.depthBits = 24;
    settings.attributeFlags = sf::ContextSettings::Core;

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    deltaTime = 0;
    time = 0;
    isRunning = true;

    // Hide the mouse and grab the input
    window->setMouseCursorVisible(false);
    window->setMouseCursorGrabbed(true);

    on_init();

}

VoxelEngine::~VoxelEngine() {

}

void VoxelEngine::update() {
    deltaTime = clock.restart().asSeconds();
    time += deltaTime;
    // Set window title to show FPS
    window->setTitle("Voxel Engine - FPS: " + std::to_string(1.0f / deltaTime));

    player->update(deltaTime);
    shaderProgram->update();
    scene->update();

}

void VoxelEngine::render() {
    // Clear the screen
    glClearColor(BG_COLOR.x,BG_COLOR.y,BG_COLOR.z,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene->render();
    // Swap buffers
    window->display();
}

void VoxelEngine::handleEvents() {
    sf::Event event;
    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed ||
           (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
            isRunning = false;
           }
        player->handle_event(event);
    }
}

void VoxelEngine::on_init() {
    textures = std::make_unique<Textures>(this);
    player = std::make_unique<Player>(this);
    shaderProgram = std::make_unique<ShaderProgram>(this);
    scene = std::make_unique<Scene>(this);
}


void VoxelEngine::run() {
    while (isRunning) {
        handleEvents();
        update();
        render();
    }

    window->close();
}


