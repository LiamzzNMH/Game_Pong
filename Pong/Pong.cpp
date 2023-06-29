#include <SFML/Graphics.hpp>
#include <cmath>
#include <SFML/Audio.hpp>
int main()
{
    srand(time(NULL));
    // Define some constants 
    const int screenWidth = 800;
    const int screenHeight = 500;
    const sf::Vector2f paddleSize(25, 100);
    const float ballRadius = 10.f;
    const float pi = 3.14159f;

    //create cua so
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), " Pong! ", sf::Style::Titlebar | sf::Style::Close);
    // paddle trai
    sf::RectangleShape leftPaddle;
    leftPaddle.setSize(paddleSize);
    leftPaddle.setFillColor(sf::Color(0, 204, 204));
    leftPaddle.setOutlineThickness(3);
    leftPaddle.setOutlineColor(sf::Color(0, 0, 0));
    leftPaddle.setPosition(0, screenHeight / 2);
    leftPaddle.setOrigin(paddleSize / 2.f);

    // paddle phai
    sf::RectangleShape rightPaddle;
    rightPaddle.setSize(paddleSize);
    rightPaddle.setFillColor(sf::Color(153, 0, 0));
    rightPaddle.setOutlineThickness(3);
    rightPaddle.setOutlineColor(sf::Color(0, 0, 0));
    rightPaddle.setPosition(screenWidth, screenHeight / 2);
    rightPaddle.setOrigin(paddleSize / 2.f);
    //Bot properties
    int botTarget = screenHeight / 2;
    float botPeriod = 0.2f;
    float botTimer = 0.f;


    // bong
    sf::CircleShape ball;
    ball.setFillColor(sf::Color(204, 255, 204));
    ball.setRadius(ballRadius);
    ball.setOutlineThickness(3);
    ball.setOutlineColor(sf::Color(0, 0, 0));
    ball.setPosition(screenWidth / 2, screenHeight / 2);

    // thong so paddle
    float paddleSpeed = 200.f;

    // thong so ball
    const float ballSpeed = 300.f;
    float angelball = -90.f;
    //Load font
    sf::Font font;
    if (!font.loadFromFile("../Data/ARCADE.TTF")) return EXIT_FAILURE;

    //Init pause Messenger
    sf::Text pauseMessenger;
    pauseMessenger.setFont(font);
    pauseMessenger.setPosition(screenWidth / 2 -250 , screenHeight / 2);
    pauseMessenger.setFillColor(sf::Color::Black);
    pauseMessenger.setCharacterSize(60);
    pauseMessenger.setString("Press space to play");


    sf::Clock clock;
    float deltaTime = 0.f;
    //Game State
    bool isPlaying = false;
    //Sound
    sf::SoundBuffer ballSFX;
    ballSFX.loadFromFile("../Data/mixkit.wav");
    sf::Sound sfx;
    sfx.setBuffer(ballSFX);
    // nhac nen
    sf::Music music;
    if (!music.openFromFile("../Data/nhacnen.ogg"))
        return -1;
    music.play();


    while (window.isOpen()) {

        deltaTime = clock.restart().asSeconds();
        sf::Event event;
     
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
          
        }

        if (isPlaying == false && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            ball.setPosition(screenWidth / 2, screenHeight / 2);
            rightPaddle.setPosition(screenWidth, screenHeight / 2);
            leftPaddle.setPosition(0, screenHeight / 2);
            do {
                angelball = rand() % 360;
            } while (angelball < 45 || (angelball > 135 && angelball < 225) || angelball>315);
            isPlaying = true;
        }
        if (isPlaying) {

            // leftpaddle moving
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                sf::Vector2f pos = leftPaddle.getPosition();
                if (pos.y + paddleSize.y / 2 - paddleSpeed * deltaTime > screenHeight) leftPaddle.setPosition(pos.x, screenHeight - paddleSize.y / 2);
                else leftPaddle.move(0, paddleSpeed * deltaTime);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                sf::Vector2f pos = leftPaddle.getPosition();
                if (pos.y - paddleSize.y / 2 - paddleSpeed * deltaTime < 0) leftPaddle.setPosition(pos.x, paddleSize.y / 2);
                else leftPaddle.move(0, -paddleSpeed * deltaTime);
            }


            // rightpaddle moving
            printf("%f\n", angelball);
            //Update target of bot
            botTimer += deltaTime;
            if (botTimer >= botPeriod) {
                if (sin(angelball * pi / 180) > 0.f) {
                    botTarget = ball.getPosition().y;
                    botTimer -= botPeriod;
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                sf::Vector2f pos = rightPaddle.getPosition();
                if (pos.y + paddleSize.y / 2 - paddleSpeed * deltaTime > screenHeight) rightPaddle.setPosition(pos.x, screenHeight - paddleSize.y / 2);
                else rightPaddle.move(0, paddleSpeed * deltaTime);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                sf::Vector2f pos = rightPaddle.getPosition();
                if (pos.y - paddleSize.y / 2 - paddleSpeed * deltaTime < 0) rightPaddle.setPosition(pos.x, paddleSize.y / 2);
                else rightPaddle.move(0, -paddleSpeed * deltaTime);
            }
            // Ball moving
            float factor = ballSpeed * deltaTime;
            ball.move(sin(angelball * pi / 180) * factor, -cos(angelball * pi / 180) * factor);

            // TOP_DOWN wall check
            if (ball.getPosition().y <= 0 || ball.getPosition().y >= screenHeight) {
                angelball = 180.f - angelball;
                sfx.play();
            }
            // va cham
            // left paddle
            if (ball.getPosition().x - ballRadius <= leftPaddle.getPosition().x + paddleSize.x / 2
                && ball.getPosition().y >= leftPaddle.getPosition().y - paddleSize.y / 2
                && ball.getPosition().y <= leftPaddle.getPosition().y + paddleSize.y / 2
                && ball.getPosition().x > leftPaddle.getPosition().x)
            {
                if (ball.getPosition().y > leftPaddle.getPosition().y) {
                    angelball = 360.f - angelball + rand() % 10;
                }
                else {
                    angelball = 360.f - angelball - rand() % 10;
                }
                sfx.play();
            }

            // right paddle
            if (ball.getPosition().x + ballRadius >= rightPaddle.getPosition().x - paddleSize.x / 1
                && ball.getPosition().y >= rightPaddle.getPosition().y - paddleSize.y / 1
                && ball.getPosition().y <= rightPaddle.getPosition().y + paddleSize.y / 1
                && ball.getPosition().x < rightPaddle.getPosition().x)
            {
                if (ball.getPosition().y > rightPaddle.getPosition().y) {
                    angelball = 360.f - angelball - rand() % 10;
                }
                else {
                    angelball = 360.f - angelball + rand() % 10;
                }
                sfx.play();
            }
            if (ball.getPosition().x <= 0) {
                pauseMessenger.setString("Red is Winner\n Press space to play");
                isPlaying = false;
            }
            else if (ball.getPosition().x >= screenWidth) {
                pauseMessenger.setString("Blue is Winner\n Press space to play");
                isPlaying = false;
            }
        }

            window.clear(sf::Color(255, 255, 255));
           

            if (isPlaying) {
                window.draw(leftPaddle);
                window.draw(rightPaddle);
                window.draw(ball);
               
            }
            else {
                window.draw(pauseMessenger);
            }
            window.display();
        }
    }

