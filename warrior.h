#pragma once
#include <math.h>
#define PI 3.141592653589793
struct warrior {
	warrior(int x, int y) {
		this->x = x;
		this->y = y;
		friction = rand() % 1000 * 0.000000045;
		color = rand() % 220;
		speedBoost = 1.0 + (0.00105 * (float)color * 2);
		friction = 1.0 - friction;
		speed = (rand() % 41 + 10) * 0.1;
		dir = (rand() % 1000 * 0.001 * PI * 2);
		aggression = ((rand() % 1991 + 500)* 0.001);
		minimumSpeed = ((rand() % 2251 + 750) * 0.001);
		if (rand() % 15 == 0) {
			aggresive = true;
			minimumSpeed *= 1.2;
			aggression *= 0.8;
		}
		else aggresive = false;
		newDir = dir;
		radius = rand() % 3 + 5;
		lifeTime = (rand() % 4500 * 0.001) + 0.5;
	}

	void rotate() {
		/*dir = newDir;*/
	}

	void move(float dist, float fElapsedTime) {
		if (dir > 2 * PI) dir -= 2 * PI;
		else if (dir < 0) dir += 2 * PI;
		if(passive > 0) passive -= fElapsedTime;
		if (hunger < 5.01) {
			hunger += fElapsedTime;
			lifeTime += fElapsedTime;
		}
		else if (aggresive) lifeTime -= fElapsedTime * 0.4;
		else lifeTime += fElapsedTime * 0.75;
		/*if (turn <= 0) turn += PI;
		if (turn < (PI * 0.5)) newDir = dir - (PI * fElapsedTime);
		else newDir = dir + (PI * fElapsedTime);*/
		x += dist * speed * fElapsedTime * cos(dir);
		y += dist * speed * fElapsedTime * sin(dir);
		//speed = speed * (1.0 - (0.09 01 * fElapsedTime));
		for(float i = minimumSpeed; i < speed; i+= 0.225) speed = speed * friction;
		if (speed > 10) speed = 10;
		shortestDist = 100000;
		if (turnTimer > 0.1) {
			float relative = dir;
			if (shortestTarget != nullptr) {
				if (aggresive == false || shortestTarget->aggresive || hunger < 5)	relative = atan2(shortestTarget->y - y, shortestTarget->x - x);
				else relative = atan2(y - shortestTarget->y, x - shortestTarget->x);
			}
			if (dir > relative) {
				if (dir - relative < PI) dir += PI * fElapsedTime * aggression;
				else dir -= PI * fElapsedTime * aggression;
			}
			else {
				if (relative - dir < PI) dir += PI * fElapsedTime * aggression;
				else dir -= PI * fElapsedTime * aggression;
			}
		}
		else turnTimer += fElapsedTime;

	}
	void rotateTowards(float dirTarget, float dirChange, float fElapsedTime) {

	}
	float bounce(float inspeed, float speedBoost2) {
		float totalSpeed = this->speed + inspeed;
		this->speed = totalSpeed * 0.5;
		float retu = totalSpeed - this->speed;
		this->speed = ((speedBoost + speedBoost2) * 0.05) + (speed * speedBoost);
		
		if (aggresive) speed *= 0.9;
		else speed *= 1.1;
		hunger = 0;
		return retu;
	}
	void collide(warrior &other) {

		float dist = sqrt(pow((other.x - x),2) + pow((other.y -y), 2));
		if (dist < shortestDist) {
			shortestDist = dist;
			shortestTarget = &other;
		}

		if (dist < other.shortestDist) {
			other.shortestDist = dist;
			other.shortestTarget = this;
			
		}
		if (dist < (other.radius + radius)) {
			float moveit = ((other.radius + radius) - dist) * 0.5;
			float relative1 = atan2(y -other.y, x -other.x);
			turnTimer = 0;
			other.turnTimer = 0;
			float relative2 = atan2(other.y - y, other.x - x);
			x += moveit * cos(relative1);
			y += moveit * sin(relative1);
			other.x += moveit * cos(relative2);
			other.y += moveit * sin(relative2);
			this->dir = relative1;
			other.dir = relative2;
			this->speed = abs(other.bounce(this->speed, speedBoost));
			this->speed = ((speedBoost + other.speedBoost) * 0.05) + (speed * speedBoost);
			if (aggresive) speed *= 0.9;
			else speed *= 1.1;
			hunger = 0;
			if (aggresive && passive <= 0) {
				other.lifeTime *= 0.5;
				other.lifeTime -= 10;
			}
			if (other.aggresive && other.passive <= 0) {
				lifeTime *= 0.5;
				lifeTime -= 10;

			}
		}
	}
	int lineX() { return (int)(cos(dir) * radius + x); }
	int lineY() { return (int)(sin(dir) * radius + y); }
	float heat() {
		return 255.0 - (255.0 / (1+speed));
	}
	float dir = 0, x = 0, y = 0, radius = 2, speed = 0;
	float speedBoost = 0, shortestDist = 100000, newDir = 0, turnTimer = 0, aggression = 0, minimumSpeed = 0, hunger = 0, lifeTime = 0, passive = 2;
	warrior* shortestTarget = nullptr;
	int color = 0, kids = 0;
	float friction = 0;
	bool aggresive;
};