/*
Everything in between is a comment.
*/
// This is a comment

/*var myHeading = document.querySelector('h1');
myHeading.textContent = 'Hello world!';*/


function multiply(num1,num2) {
  let result = num1 * num2;
  return result;
}
/*
document.querySelector('html').onclick = function() { //var myHTML = document.querySelector('html');myHTML.onclick = function() {};
    alert('Ouch! Stop poking me!');
} //alert跳出視窗裡面為函式內容*/
  
// Image switcher code
let myImage = document.querySelector('img');

myImage.onclick = function() {
    let mySrc = myImage.getAttribute('src');
    if(mySrc === 'images/dog_1.jpg') {
      myImage.setAttribute ('src','images/dog_2.jpg');
    } else {
      myImage.setAttribute ('src','images/dog_1.jpg');
    }
}

// Personalized welcome message code
let myButton = document.querySelector('button');
let myHeading = document.querySelector('h1');

function setUserName() {
  let myName = prompt('Please enter your name.');
  if(!myName || myName === null) {
    setUserName();
  } else {
    localStorage.setItem('name', myName);
    myHeading.innerHTML = 'Aloha！' + myName;
  }
}

if(!localStorage.getItem('name')) {
  setUserName();
} else {
  let storedName = localStorage.getItem('name');
  myHeading.innerHTML = 'Welcome ~ ' + storedName;
}

myButton.onclick = function() {
  setUserName();
}

const canvas = document.querySelector('.myCanvas');
const width = canvas.width = 600;
const height = canvas.height = 200;
const ctx = canvas.getContext('2d');

function degToRad(degrees) {
  return degrees * Math.PI / 180;
};

let posX = 0;
let posY = height/2;
let radius = 30;
let order = 0;
let range = 20;
let wallX = 0;
let wallY_up = 0;
let wallY_down = height;
let PacmanColor = 'yellow';
let color = '';

let squareX = [];
for(var i=3; i<=width-3; i+=22) squareX.push(i);

function open_big(X, Y) {
  ctx.fillStyle = PacmanColor;
  ctx.beginPath();
  ctx.arc(posX, posY, radius, degToRad(-45), degToRad(45), true);
  ctx.lineTo(posX, posY);
  ctx.fill();
  
  ctx.fillStyle = 'black';
  ctx.beginPath();
  ctx.arc(posX, posY-radius/3*2, 3, degToRad(0), degToRad(360), true);
  ctx.fill();
};

function open_middle(X, Y) {
  ctx.fillStyle = PacmanColor;
  ctx.beginPath();
  ctx.arc(posX, posY, radius, degToRad(-30), degToRad(30), true);
  ctx.lineTo(posX, posY);
  ctx.fill();
  
  ctx.fillStyle = 'black';
  ctx.beginPath();
  ctx.arc(posX, posY-radius/3*2, 3, degToRad(0), degToRad(360), true);
  ctx.fill();
};

function open_small(X, Y) {
  ctx.fillStyle = PacmanColor;
  ctx.beginPath();
  ctx.arc(posX, posY, radius, degToRad(-15), degToRad(15), true);
  ctx.lineTo(posX, posY);
  ctx.fill();
  
  ctx.fillStyle = 'black';
  ctx.beginPath();
  ctx.arc(posX, posY-radius/3*2, 3, degToRad(0), degToRad(360), true);
  ctx.fill();
}

function square(X, Y, W, H, color) {
  ctx.fillStyle = color;
  ctx.fillRect(X-W/2, Y-H/2, W, H);
};

function draw() {
  ctx.fillStyle = 'DarkGrey';
  ctx.fillRect(0, 0, width, height);
  
  for(let i of squareX) {
    if(i > posX) square(i, posY, 6, 6, 'red');
  };
  
  for(var i=0; i<width; i+=14) {
	square(i+6, wallY_up+6, 12, 12, 'DarkSlateGrey');
	square(i+6, wallY_down-6, 12, 12, 'DarkSlateGrey');
  };
  
  switch (order) {
    case 0:
	  open_big(posX, posY);
	  break;
	case 1:
	  open_middle(posX, posY);
	  break;
	case 2:
	  open_small(posX, posY);
	  break;
  };
  
  if(posX > (width + radius/2)) posX = 0 - radius;
  else posX += 2;
  
  if(posX%range === 0) {
	if(order === 2) order = 0;
    else order += 1;
  };
  
  requestAnimationFrame(draw);
};

draw();
