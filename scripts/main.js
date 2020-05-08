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
    myHeading.innerHTML = 'Welcome, ' + myName;
  }
}

if(!localStorage.getItem('name')) {
  setUserName();
} else {
  let storedName = localStorage.getItem('name');
  myHeading.innerHTML = 'Aloha！ ' + storedName;
}

myButton.onclick = function() {
  setUserName();
}

