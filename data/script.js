function updateSliderPWM(element) {
    var sliderValue = element.value;
    document.getElementById("textSliderValue").innerHTML = sliderValue;
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/slider?value="+sliderValue);
    xhr.send();
}

function start() {
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/start");
    xhr.send();
} 
