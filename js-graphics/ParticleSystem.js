class Vector2 {
  constructor(x, y) {
    this.x = x;
    this.y = y;
  }
  length() {
    return Math.sqrt(this.x * this.x + this.y * this.y);
  }
  sqrLength() {
    return this.x * this.x + this.y * this.y;
  }
  normalize() {
    var inv = 1 / this.length();
    return new Vector2(this.x * inv, this.y * inv);
  }
  negate() {
    return new Vector2(-this.x, -this.y);
  }
  add(v) {
    return new Vector2(this.x + v.x, this.y + v.y);
  }
  subtract(v) {
    return new Vector2(this.x - v.x, this.y - v.y);
  }
  multiply(f) {
    return new Vector2(this.x * f, this.y * f);
  }
  divide(f) {
    var invf = 1 / f;
    return new Vector2(this.x * invf, this.y * invf);
  }
  dot(v) {
    return this.x * v.x + this.y * v.y;
  }
}

Vector2.zero = new Vector2(0, 0);
class Color {
  constructor(r, g, b) {
      this.r = r;
      this.g = g;
      this.b = b;
    }
    // copy() {
    //   return new Color(this.r, this.g, this.b);
    // }
  add(c) {
    return new Color(this.r + c.r, this.g + c.g, this.b + c.b);
  }
  multiply(s) {
    return new Color(this.r * s, this.g * s, this.b * s);
  }
  modulate(c) {
    return new Color(this.r * c.r, this.g * c.g, this.b * c.b);
  }
}

Color.black = new Color(0, 0, 0);
Color.white = new Color(1, 1, 1);
Color.red = new Color(1, 0, 0);
Color.green = new Color(0, 1, 0);
Color.blue = new Color(0, 0, 1);

class Particle {
  constructor(position, velocity, acceleration, life, color, size) {
    this.position = position;
    this.velocity = velocity;
    this.acceleration = acceleration;
    this.life = life;
    this.color = color;
    this.size = size;
    this.age = 0;
  }

  aging(dt) {
    this.age += dt;
  }
  dead() {
    return this.age >= this.life;
  }
}

function sampleDirection() {
  const theta = Math.random() * 2 * Math.PI;
  return new Vector2(Math.cos(theta), Math.sin(theta));
}

function sampleColor() {
  //const theta = Math.random();
  return new Color(Math.random(), Math.random(), Math.random());
  //return new Vector2(Math.cos(theta), Math.sin(theta));
}

function sampleInterval(start, end){
   return start + (end - start)*Math.random();
}

function ChamberBox(x1, y1, x2, y2) {
  return function(p) {
    if (p.position.x - p.size < x1 || p.position.x + p.size > x2) {
      p.velocity.x = -p.velocity.x;
    }
    if (p.position.y - p.size < y1 || p.position.y + p.size > y2) {
      p.velocity.y = -p.velocity.y;
    }
  };
}
class ParticleSystem {
  constructor() {
    this.particles = new Array();
    this.effects = new Array();
    this.gravity = new Vector2(0, 10);
  }
  applyGravity() {
    for (let p of this.particles)
      p.acceleration = this.gravity;
  }
  addEffect(e) {
    this.effects.push(e);
  }
  applyEffects() {
    for (let e of this.effects)
      for (let p of this.particles)
        e(p);
  }
  emit(p) {
    this.particles.push(p);
  }
  aging(dt) {
    for (let i in this.particles) {
      let p = this.particles[i];
      p.aging(dt);
      if (p.dead()) this.remove(i);
    }
  }

  remove(index) {
    if (this.particles.length > 1)
      this.particles[index] = this.particles[this.particles.length - 1];
    this.particles.pop();
  }
  kinematics(dt) {
    for (let p of this.particles) {
      p.position = p.position.add(p.velocity.multiply(dt));
      p.velocity = p.velocity.add(p.acceleration.multiply(dt));
    }
  }
  render(ctx) {
    for (let p of this.particles) {
      const alpha = 1 - p.age / p.life;
      //console.log(p);
      ctx.fillStyle = "rgba(" + Math.floor(p.color.r * 255) + "," + Math.floor(p.color.g * 255) + "," + Math.floor(p.color.b * 255) + "," + alpha.toFixed(2) + ")";
      ctx.beginPath();
      ctx.arc(p.position.x, p.position.y, p.size, 0, Math.PI * 2, true);
      ctx.closePath();
      ctx.fill();
    }
  }
}

function init() {
  const canvas = document.getElementById('testCanvas');
  const ctx = canvas.getContext('2d');
  const ps = new ParticleSystem();
  ps.addEffect(ChamberBox(0, 0, canvas.width, canvas.height));

  const dt = 0.1;
  const initPosition = new Vector2(200, 200);
  
  let newMousePosition = Vector2.zero;
  let oldMousePosition = Vector2.zero;
  
  canvas.onmousemove = function(e) {
      newMousePosition = new Vector2(e.offsetX, e.offsetY);
  };

  window.requestAnimationFrame(draw);

  function draw() {
    const velocity = newMousePosition.subtract(oldMousePosition).multiply(5).add(sampleDirection().multiply(10));
    const acceleration = 0;
    const life = sampleInterval(10, 20);
    const color = sampleColor();
    const size = sampleInterval(3, 8);
      const p = new Particle(newMousePosition, velocity, acceleration, life, color, size);
      oldMousePosition = newMousePosition;
    //this is a hack to create motion blur
    ctx.fillStyle = "rgba(0, 0, 0, 0.3)";
    ctx.fillRect(0, 0, canvas.width, canvas.height);
    // ctx.claerRect(0, 0, canvas.width, canvas.height);

    ps.emit(p);
    ps.aging(dt);
    ps.applyGravity();
    ps.applyEffects();
    ps.kinematics(dt);
    ps.render(ctx);
    window.requestAnimationFrame(draw);
  }
}



init();
