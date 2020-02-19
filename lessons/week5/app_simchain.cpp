#include "p5.hpp"

float gRadius = 20;
float s1, s2;
float gravity = 4.0;    // 9.0
float mass = 1.75;       // 2.0

PixRGBA  randomColor()
{
    int r = random(30,255);
    int g = random(30,255);
    int b = random(30,255);
    
    return PixRGBA(r,g,b,126);
}





struct Spring2D {
    int x;
    int y;
    int radius;
    int vx;
    int vy;
    float mass;
    float gravity;
    float stiffness;
    float damping;
    PixRGBA color;

    Spring2D(int xpos, int ypos, float m, float g, PixRGBA c = randomColor())
    {
        x = xpos;   // The x- and y-coordinates
        y = ypos;
        vx = 0;     // The x- and y-axis velocities
        vy = 0;
        mass = m;
        gravity = g;
        radius = radius;
        stiffness = 0.2;
        damping = 0.7;
        color = c;
    }

    void update(int targetX, int targetY)
    {
        float forceX = (targetX - x) * stiffness;
        float ax = forceX / mass;
        
        vx = damping * (vx + ax);
        x = x + vx;
        float forceY = (targetY - y) * stiffness;
        forceY = forceY + gravity;
        float ay = forceY / mass;
        vy = damping * (vy + ay);
        y = y + vy;
    }

    void display(int nx, int ny)
    {
        noStroke();
        fill(color, 126)
        ellipse(x, y, radius * 2, radius * 2);
        stroke(255);
        line(x, y, nx, ny);
    }

};









Sprint2D springs = {};
Sprint2D * headSpring = nullptr;

void addSpring()
{
    local aspring = Spring2D(0.0, width / 2, mass, gravity);
    
    if not headSpring then
        headSpring = aspring
    else
        table.insert(springs, aspring)
    end
}

void removeSpring()
{}
    if #springs > 1 then
        springs[#springs] = nil;
    end
}

void reset()
{
  // Inputs: x, y, mass, gravity
  springs = {}
  headSpring = nil;

  addSpring();
  addSpring();
}

int T_SP = ' ';
int VK_UP = 38;
int VK_DOWN = 40;

void keyReleased(const KeyEvent &event)
{
    //print("keyReleased: ", event, keyCode)
    if (keyCode == VK_UP) {
        addSpring();
    } else if (keyCode == VK_DOWN) {
        removeSpring();
    }
}

void keyTyped(const KeyEvent &event)
{
    //print("keyTyped: ", event, keyCode)
    if (keyCode == T_SP) {
        reset();
    }
}

void draw()
{
  background(0);
  
  if not mouseX then return end


    headSpring:update(mouseX, mouseY);
    headSpring:display(mouseX, mouseY);
    
    -- iterate through rest of springs
    local currentSpring = headSpring
    for _, spring in ipairs(springs) do
        spring:update(currentSpring.x, currentSpring.y);
        spring:display(currentSpring.x, currentSpring.y);
        currentSpring = spring
    end

    stats:draw()
}

void setup()
{
    createCanvas(1280, 1024);
    setFrameRate(30);
    setFill(0x7fffffff);

  reset();
}





go {width = 1280, height=1024, frameRate=30}