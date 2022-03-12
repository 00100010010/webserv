const cvs = document.getElementById('cvs');
const ctx = cvs.getContext('2d');
const selector = 400;
// ctx.globalCompositeOperation = 'destination-out';
ctx.lineWidth = 12
let w = cvs.width = window.innerWidth,
    h = cvs.height = window.innerHeight,
    amount = 400,
    bgColor = '#000000',
    rd = (max) => max * Math.random();
rdInt = (max) => ~~rd(max),
    rdColor = () => `rgba(${rdInt(255)}, ${rdInt(255)}, ${rdInt(255)})`,
    dist = (x1, y1, x2, y2) => Math.sqrt((x1 - x2) ** 2 + (y1 - y2) ** 2);
class particle {
    static maxSpeed = 1;
    static maxRadius = 3;
    static maxDist = 200;
    constructor(x, y) {
        this.x = x;
        this.y = y;
        this.r = rd(particle.maxRadius);
        this.color = particle.color();
        this.angle = rd(0.5);
        this.speed = rd(particle.maxSpeed);
    }
    checkCollision() {
        if (this.x + this.r >= w || this.x - this.r <= 0) {
            this.angle = Math.PI - this.angle;
            this.move();
        }
        if (this.y + this.r >= h || this.y - this.r <= 0) {
            this.angle *= -1;
            this.move();
        }
    }
    move() {
        this.x += Math.cos(this.angle) * this.speed;
        this.y -= Math.sin(this.angle) * this.speed;
    }
    render() {
        ctx.beginPath();
        ctx.fillStyle = this.color;
        ctx.arc(this.x, this.y, this.r, 0, 2 * Math.PI);
        ctx.fill();
        ctx.closePath();
    }
}
particle.clickedAmount = 1;
particle.color = () => 'white';
let particles = [];
for (let i = 0; i < amount; i++) particles.push(new particle(rd(w), rd(h)))

function loop() {
    ctx.fillStyle = bgColor;
    ctx.fillRect(0, 0, w, h);
    let len = particles.length;
    for (let i = 0; i < len; i++) {
        let p = particles[i];
        p.move();
        p.checkCollision();

        for (let j = 0; j < i; j++) {
            let p1 = particles[j];
            let d = dist(p.x, p.y, p1.x, p1.y);
            if (d < particle.maxDist) {
                ctx.beginPath();
                ctx.globalAlpha = 1 - d / particle.maxDist
                ctx.strokeStyle = p1.color;
                ctx.moveTo(p.x, p.y);
                ctx.lineTo(p1.x, p1.y)
                ctx.stroke();

                ctx.closePath();
            }
        }
        ctx.globalAlpha = 1;
        p.render();
    }
    requestAnimationFrame(loop)
}
loop()

window.addEventListener('click', (e) => {
    let x = e.clientX - cvs.getBoundingClientRect().left,
        y = e.clientY - cvs.getBoundingClientRect().top;
    for (let i = 0; i < particle.clickedAmount; i++) particles.push(new particle(x, y))
})
cvs.addEventListener('mousemove', (e) => {
    let x = e.clientX - cvs.getBoundingClientRect().left,
        y = e.clientY - cvs.getBoundingClientRect().top;
    particles[0].speed = 0;
    particles[0].x = x;
    particles[0].y = y;
    console.log('fef')
})
cvs.addEventListener('mouseleave', () => {
    particles[0].speed = rd(particle.maxSpeed)
})
// selector.value = amount;
// selector.addEventListener('input', (e) => {
//     let a = +selector.value;
//     amount = a;
//     if (a < particles.length) {
//         particles = particles.slice(0, a);
//     } else {
//         for (let i = 0; i < a - particles.length; i++) particles.push(new particle(rd(w), rd(h)))
//     }

// })


var inputs = document.querySelectorAll( '.inputfile' );
Array.prototype.forEach.call( inputs, function( input )
{
	var label	 = input.nextElementSibling,
		labelVal = label.innerHTML;

	input.addEventListener( 'change', function( e )
	{
		var fileName = '';
		if( this.files && this.files.length > 1 )
			fileName = ( this.getAttribute( 'data-multiple-caption' ) || '' ).replace( '{count}', this.files.length );
		else
			fileName = this.files[0].name;

		if( fileName )
			label.innerHTML = fileName;
		else
			label.innerHTML = labelVal;
	});
});