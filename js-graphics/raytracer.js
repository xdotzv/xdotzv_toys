class Vector3 {
  constructor(x, y, z) {
      this.x = x;
      this.y = y;
      this.z = z;
    }
    // copy() {
    //   return new Vector3(this.x, this.y, this.z);
    // }
  sqrLength() {
    return this.x * this.x + this.y * this.y + this.z * this.z;
  }
  length() {
    return Math.sqrt(this.sqrLength());
  }
  normalize() {
    var inv = 1 / this.length();
    return new Vector3(this.x * inv, this.y * inv, this.z * inv);
  }
  negate() {
    return new Vector3(-this.x, -this.y, -this.z);
  }
  add(v) {
    return new Vector3(this.x + v.x, this.y + v.y, this.z + v.z);
  }
  subtract(v) {
    return new Vector3(this.x - v.x, this.y - v.y, this.z - v.z);
  }
  multiply(f) {
    return new Vector3(this.x * f, this.y * f, this.z * f);
  }
  divide(f) {
    const invf = 1 / f;
    return new Vector3(this.x * invf, this.y * invf, this.z * invf);
  }
  dot(v) {
    return this.x * v.x + this.y * v.y + this.z * v.z;
  }
  cross(v) {
    return new Vector3(-this.z * v.y + this.y * v.z, this.z * v.x - this.x * v.z, -this.y * v.x + this.x * v.y);
  }
}

class Ray3 {
  constructor(origin, direction) {
    this.origin = origin;
    this.direction = direction.normalize();
  }
  getPoint(t) {
    return this.origin.add(this.direction.multiply(t));
  }
}

class Sphere {
  constructor(center, radius) {
      this.center = center;
      this.radius = radius;
    }
    // copy() {
    //   return new Sphere(this.center.copy(), this.radius.copy());
    // }
  initialize() {
    this.sqrRadius = this.radius * this.radius;
  }
  intersect(ray) {
    const v = ray.origin.subtract(this.center);
    const a0 = v.sqrLength() - this.sqrRadius;
    const DdotV = ray.direction.dot(v);
    if (DdotV <= 0) {
      const discr = DdotV * DdotV - a0;
      if (discr >= 0) {
        const geometry = this;
        const distance = -DdotV - Math.sqrt(discr);
        const position = ray.getPoint(distance);
        const normal = position.subtract(this.center).normalize();
        return new IntersectResult(geometry, distance, position, normal);
      }
    }
    return IntersectResult.noHit;
  }
}

class IntersectResult {
  constructor(geometry, distance, position, normal) {
    this.geometry = geometry;
    this.distance = distance;
    this.position = position;
    this.normal = normal;
  }
}
IntersectResult.noHit = new IntersectResult(null, 0, Vector3.zero, Vector3.zero);

class PerspectiveCamera {
  // fov: use degree
  constructor(eye, front, up, fov) {
    this.eye = eye;
    this.front = front.normalize();
    // this.refUp = up;
    this.up = up.normalize();
    this.fov = fov;
  }
  initialize() {
    //this.right = this.front.cross(this.refUp);
    //this.up = this.right.cross(this.front);
    this.right = this.front.cross(this.up);
    this.fovScale = Math.tan(this.fov * 0.5 * Math.PI / 180) * 2;
  }
  generateRay(x, y) {
    const r = this.right.multiply((x - 0.5) * this.fovScale);
    const u = this.up.multiply((y - 0.5) * this.fovScale);
    return new Ray3(this.eye, this.front.add(r).add(u).normalize());
  }
}

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


class CheckerMaterial {

  constructor(scale, reflectiveness) {
    this.scale = scale;
    this.reflectiveness = reflectiveness;
  }

  sample(ray, position, normal) {
    if (((Math.floor(position.x * this.scale) + Math.floor(position.z * this.scale)) % 2 + 2) % 2 == 1)
      return Color.white;
    else
      return Color.black;
  }
}


// global temp
const lightDir = new Vector3(1, 1, 1).normalize();
const lightColor = Color.white;


class PhongMaterial {
  constructor(diffuse, specular, shininess, reflectiveness) {
    this.diffuse = diffuse;
    this.specular = specular;
    this.shininess = shininess;
    this.reflectiveness = reflectiveness;
  }

  sample(ray, position, normal) {
    const NdotL = normal.dot(lightDir);
    const H = (lightDir.subtract(ray.direction)).normalize();
    const NdotH = normal.dot(H);
    const diffuseTerm = this.diffuse.multiply(Math.max(NdotL, 0));
    const specularTerm = this.specular.multiply(Math.pow(Math.max(NdotH, 0), this.shininess));
    return lightColor.modulate(diffuseTerm.add(specularTerm));
  }
}




class Plane {
  constructor(normal, d) {
    this.normal = normal.normalize();
    this.d = d;
  }

  initialize() {
    this.position = this.normal.multiply(this.d);
  }

  intersect(ray) {
    var a = ray.direction.dot(this.normal);
    if (a >= 0)
      return IntersectResult.noHit;

    var b = this.normal.dot(ray.origin.subtract(this.position));
    var result = new IntersectResult();
    result.geometry = this;
    result.distance = -b / a;
    result.position = ray.getPoint(result.distance);
    result.normal = this.normal;
    return result;
  }

}

class Union {
  constructor(geometries) {
    this.geometries = geometries;
  }

  initialize() {
    for (let g of this.geometries)
      g.initialize();
  }

  intersect(ray) {
    let minDistance = Infinity;
    let minResult = IntersectResult.noHit;
    for (let g of this.geometries) {
      const result = g.intersect(ray);
      if (result.geometry && result.distance < minDistance) {
        minDistance = result.distance;
        minResult = result;
      }
    }
    return minResult;
  }
}

function rayTraceRecursive(scene, ray, maxReflect) {
    var result = scene.intersect(ray);
     
    if (result.geometry) {
        const reflectiveness = result.geometry.material.reflectiveness;
        let color = result.geometry.material.sample(ray, result.position, result.normal);
        color = color.multiply(1 - reflectiveness);
         
        if (reflectiveness > 0 && maxReflect > 0) {
            const r = result.normal.multiply(-2 * result.normal.dot(ray.direction)).add(ray.direction);
            ray = new Ray3(result.position, r);
            const reflectedColor = rayTraceRecursive(scene, ray, maxReflect - 1);
            color = color.add(reflectedColor.multiply(reflectiveness));
        }
        return color;
    }
    else
        return Color.black;
}

function render(canvas, scene, camera, maxDepth) {
  const ctx = canvas.getContext("2d");
  const w = canvas.attributes.width.value;
  const h = canvas.attributes.height.value;
  ctx.fillStyle = "rgb(0,0,0)";
  ctx.fillRect(0, 0, w, h);
  const imgdata = ctx.getImageData(0, 0, w, h);
  const pixels = imgdata.data;

  scene.initialize();
  camera.initialize();

  var i = 0;
  for (var y = 0; y < h; y++) {
    var sy = 1 - y / h;
    for (var x = 0; x < w; x++) {
      const sx = x / w;
      const ray = camera.generateRay(sx, sy);
      //var result = scene.intersect(ray);
      const color = rayTraceRecursive(scene, ray, 5);
      //if (result.geometry) {
      //  const color = result.geometry.material.sample(ray, result.position, result.normal);
        pixels[i] = color.r * 255;
        pixels[i + 1] = color.g * 255;
        pixels[i + 2] = color.b * 255;
        pixels[i + 3] = 255;
      //}
      i += 4;
    }
  }
  ctx.putImageData(imgdata, 0, 0);
}

const s1 = new Sphere(new Vector3(10, 10, -10), 5);
const s2 = new Sphere(new Vector3(-10, 10, -10), 5);
s1.material = new PhongMaterial(Color.red, Color.white, 16, 0.25);
s2.material = new PhongMaterial(Color.blue, Color.white, 16, 0.25);

const plane = new Plane(new Vector3(0, 1, 0), 0);
plane.material = new CheckerMaterial(0.1, 0.5);

render(
  document.getElementById('testCanvas'),
  new Union([s1, s2, plane]),
  new PerspectiveCamera(new Vector3(0, 30, -10), new Vector3(0, -1, 0), new Vector3(0, 0, 1), 90),
  20);
