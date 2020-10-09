#include "ShMUp.h"
#include <sstream>
#include <iostream>

using namespace sf;


ShMUp::ShMUp()  : window(VideoMode(640,480), "ShMUpStrike!"){
    camera.setSize(window.getSize().x,window.getSize().y);
    camera.setCenter(window.getSize().x/2,window.getSize().y/2);
    window.setView(camera);
    change = Time::Zero;
    playTex.loadFromFile("TurboKat.png");
    enTex.loadFromFile("Sinestrum.png");

    player.setTexture(playTex);
    player.setScale(.2f, .2f);
    for(int i= 0; i<20; i++){
        spawnEnemy(Vector2f(400-(40.f*i), 0-(60.f*i)));
        enX.push_back(160);
    }
    score = 0;
    lives=2;


    HUD.setPosition(450.f,360.f);
    displayFont.loadFromFile("C:\\Windows\\Fonts\\Arial.ttf");
    HUD.setFont(displayFont);
    HUD.setCharacterSize(24);
    updateHUD();


    player.setPosition(320.f-(.5*player.getGlobalBounds().width), 480.f-player.getGlobalBounds().height);
    hasFired=false;

}

void ShMUp::updateHUD(){
    ss.str("");
    ss <<lives;
    HUD.setString("Lives: " + ss.str() + "\n");
    ss.str("");
    ss << score;
    HUD.setString(HUD.getString()+ "Score: " + ss.str());
}

void ShMUp::takeLife() {
    if(lives == 0){
        window.close();
    }
    lives--;
}

void ShMUp::fire(){
    CircleShape newBull;
    newBull.setRadius(2.f);
    newBull.setFillColor(Color::Black);
    newBull.setPosition(player.getPosition().x +(player.getGlobalBounds().width/2), player.getPosition().y- 2.f);
    bullets.push_back(newBull);
    hasFired =true;
}

void ShMUp::spawnEnemy(Vector2f position){
    Sprite newEnemy;
    newEnemy.setTexture(enTex);
    newEnemy.setScale(.8f, .8f);
    newEnemy.setPosition(position);
    enemies.push_back(newEnemy);
}

void ShMUp::processEvents(){
    Event event;

    while(window.pollEvent(event)){
        if(event.type == Event::Closed){
            window.close();
        }

        if(event.type == Event::Resized){
            camera.setSize(window.getSize().x,window.getSize().y);
            camera.setCenter(window.getSize().x/2,window.getSize().y/2);
            window.setView(camera);
        }

        if(event.type == Event::KeyPressed){

            /*if(event.key.code == Keyboard::Delete){
               takeLife();
            }*/

            if(event.key.code == Keyboard::D){
                isMovingRight = true;
            }
            if(event.key.code == Keyboard::A){
                isMovingLeft = true;
            }

            if(event.key.code == Keyboard::Space){
                if(!hasFired){
                    fire();
                }
            }

        }

        if(event.type == Event::KeyReleased){
            if(event.key.code == Keyboard::Escape){
                window.close();
            }
            if(event.key.code == Keyboard::D){
                isMovingRight = false;
            }
            if(event.key.code == Keyboard::A){
                isMovingLeft = false;
            }
            if(event.key.code == Keyboard::Space){
                hasFired = false;
            }
        }
    }
    for(int i=0;i<enemies.size();i++){
        if(enemies[i].getPosition().y > 480 ){
            enemies.erase(enemies.begin()+i);
            break;
        }
        if (enemies[i].getPosition().x <= 40 ){
            enX[i]= 160;
            enemies[i].move(enX[i]*change.asSeconds(),40*change.asSeconds());
        }else if (enemies[i].getPosition().x +enemies[i].getGlobalBounds().width >=600) {
            enX[i] = -160;
            enemies[i].move(enX[i]*change.asSeconds(),40*change.asSeconds());
        } else {
            enemies[i].move(enX[i]*change.asSeconds(), 40*change.asSeconds());
        }
        if((enemies[i].getPosition().y + (.85*enemies[i].getGlobalBounds().height) > player.getPosition().y)
                                        && ((enemies[i].getPosition().y + (.85*enemies[i].getGlobalBounds().height)) < (player.getPosition().y+(.85*player.getGlobalBounds().height)))){
            if((enemies[i].getPosition().x < (player.getPosition().x+player.getGlobalBounds().width))
                                             && (enemies[i].getPosition().x >player.getPosition().x)){

                takeLife();
                enemies.erase(enemies.begin()+i);
                enX.erase(enX.begin()+i);
                spawnEnemy(Vector2f(0,-20.f));
                enX.push_back(160);
                updateHUD();
                break;
            } else if(((enemies[i].getPosition().x +enemies[i].getGlobalBounds().width)< (player.getPosition().x+player.getGlobalBounds().width))
                                             && ((enemies[i].getPosition().x+enemies[i].getGlobalBounds().width) >player.getPosition().x)){
                takeLife();
                enemies.erase(enemies.begin()+i);
                enX.erase(enX.begin()+i);
                spawnEnemy(Vector2f(0,-20.f));
                enX.push_back(160);
                updateHUD();
                break;
            }

        }
    }
     if((player.getPosition().x + player.getGlobalBounds().width)>= 640){
         isMovingRight = false;
     }

     if(player.getPosition().x <= 0){
        isMovingLeft = false;
     }
    if(isMovingRight){
        player.move(120*change.asSeconds(), 0);
    }
    if(isMovingLeft){
        player.move(-120*change.asSeconds(), 0);
    }

    for(int i = 0; i < bullets.size(); i++){
       if(bullets[i].getPosition().y + 4.f< 0){
            bullets.erase(bullets.begin()+i);
            break;
        }
        bullets[i].move(0.f,-400.f*change.asSeconds());

        for(int j = 0; j < enemies.size(); j++){
            if((bullets[i].getPosition().y + (.85*bullets[i].getGlobalBounds().height) > enemies[j].getPosition().y)
                                            && ((bullets[i].getPosition().y + (.85*bullets[i].getGlobalBounds().height)) < (enemies[j].getPosition().y+(.85*enemies[j].getGlobalBounds().height)))){
                if((bullets[i].getPosition().x < (enemies[j].getPosition().x+enemies[j].getGlobalBounds().width))
                                                 && (bullets[i].getPosition().x >enemies[j].getPosition().x)){


                    enemies.erase(enemies.begin()+j);
                    enX.erase(enX.begin()+j);
                    bullets.erase(bullets.begin()+i);
                    score += 100;
                    spawnEnemy(Vector2f(0,-20));
                    enX.push_back(160);
                    updateHUD();
                    break;
                }
                if(((bullets[i].getPosition().x +bullets[i].getGlobalBounds().width)< (enemies[j].getPosition().x+enemies[j].getGlobalBounds().width))
                                                 && ((bullets[i].getPosition().x+bullets[i].getGlobalBounds().width) >enemies[j].getPosition().x)){
                    enemies.erase(enemies.begin()+j);
                    enX.erase(enX.begin()+j);
                    bullets.erase(bullets.begin()+i);
                    score += 100;
                    spawnEnemy(Vector2f(0,-20));
                    enX.push_back(160);
                    updateHUD();
                    break;

                }
            }
        }
    }
}

void ShMUp::render(){

    window.clear(Color(0x00,0x00,0xFF,0x00));
    for(int i =0; i < enemies.size(); i++){
        window.draw(enemies[i]);
    }
    for(int i =0; i < bullets.size(); i++){
        window.draw(bullets[i]);
    }
    window.draw(player);
    window.draw(HUD);
    window.display();
}
void ShMUp::run(){

    while(window.isOpen()){
        processEvents();
        render();
        change = clock.restart();
    }

}
