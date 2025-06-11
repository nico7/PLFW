const sampling_period_ms = 200;
const slow_sampling_period_ms = 500;

const data_buffer = 100;
const half_second_sampling = 500;

const width = window.innerWidth;
const height = window.innerHeight;



let showingTecChart = false;
const tecOutputContainer = document.getElementById('tecOutputContainer');
const tecToggleButton = document.getElementById('toggleTecTempButton');

tecToggleButton.addEventListener('click', () => {
  // Clear the container
  tecOutputContainer.innerHTML = '';

  if (showingTecChart) {
    // Revert to number output
    const outputField = document.createElement('span');
    outputField.id = 'tecOutputField';
    outputField.textContent = ''; // Or set dynamic value here
    tecOutputContainer.appendChild(outputField);

    // Destroy the Highcharts instance if it exists
    Highcharts.charts.forEach(chart => {
      if (chart && chart.renderTo.id === 'TEC Temperature') {
        chart.destroy();
      }
    });
  } else {
    // Add chart container
    const chartDiv = document.createElement('div');
    chartDiv.id = 'TEC Temperature';
    chartDiv.className = 'container';
    tecOutputContainer.appendChild(chartDiv);

    // Create the chart
    var tecTemperatureChart = new Highcharts.Chart({
      chart: {
        renderTo: 'TEC Temperature',
        backgroundColor: '#4A4A4A'
      },
      title: {
        text: 'TEC Temperature',
        style: {
          color: '#FFFFFF'
        }
      },
      series: [{
        showInLegend: false,
        data: []
      }],
      plotOptions: {
        line: {
          animation: false,
          dataLabels: { enabled: false }
        },
        series: { color: '#f6a82e' }
      },
      xAxis: {
        labels: {
          style: {
            color: '#FFFFFF'
          }
        },
        type: 'datetime',
        dateTimeLabelFormats: { second: '%S' }
      },
      yAxis: {
        min: 300,
        max: 700,
        labels: {
          style: {
            color: '#FFFFFF'
          }
        },
        title: {
          text: 'TEC temperature value',
          style: {
            color: '#FFFFFF'
          }
        }
      },
      credits: { enabled: false }
    });
    setInterval(function () {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          var x = (new Date()).getTime(),
            y = parseFloat(this.responseText);
          //console.log(this.responseText);
          if (tecTemperatureChart.series[0].data.length > data_buffer) {
            tecTemperatureChart.series[0].addPoint([x, y], true, true, false);
          } else {
            tecTemperatureChart.series[0].addPoint([x, y], true, false, false);
          }
        }
      };
      xhttp.open("GET", "/tectemperature", true);
      xhttp.send();
    }, sampling_period_ms);
  }

  showingTecChart = !showingTecChart;
});



// This part below is for updating just the numeric value
setInterval(function () {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function () {
    if (this.readyState === 4 && this.status === 200) {
      var x = (new Date()).getTime();
      var y = parseFloat(this.responseText);

      if (showingTecChart) {
        // Update the chart
        if (tecTemperatureChart.series[0].data.length > data_buffer) {
          tecTemperatureChart.series[0].addPoint([x, y], true, true, false);
        } else {
          tecTemperatureChart.series[0].addPoint([x, y], true, false, false);
        }
      } else {
        // Update the numeric field
        const outputField = document.getElementById('tecOutputField');
        if (outputField) {
          outputField.textContent = y.toFixed(2); // Optional: format to 2 decimal places
        }
      }
    }
  };
  xhttp.open("GET", "/tectemperature", true);
  xhttp.send();
}, slow_sampling_period_ms);


let showingHeaterChart = false;
const heaterOutputContainer = document.getElementById('heaterOutputContainer');
const heaterToggleButton = document.getElementById('toggleHeaterButton');

heaterToggleButton.addEventListener('click', () => {
  // Clear the container
  heaterOutputContainer.innerHTML = '';

  if (showingHeaterChart) {
    // Revert to number output
    const outputField = document.createElement('span');
    outputField.id = 'heaterOutputField';
    outputField.textContent = ''; // Or set dynamic value here
    heaterOutputContainer.appendChild(outputField);

    // Destroy the Highcharts instance if it exists
    Highcharts.charts.forEach(chart => {
      if (chart && chart.renderTo.id === 'Heater Current') {
        chart.destroy();
      }
    });
  } else {
    // Add chart container
    const chartDiv = document.createElement('div');
    chartDiv.id = 'Heater Current';
    chartDiv.className = 'container';
    heaterOutputContainer.appendChild(chartDiv);

    var heaterCurrentChart = new Highcharts.Chart({
      chart: {
        renderTo: 'Heater Current',
        backgroundColor: '#4A4A4A'
      },
      title: {
        text: 'Heater Current',
        style: {
          color: '#FFFFFF'
        }
      },
      series: [{
        showInLegend: false,
        data: []
      }],
      plotOptions: {
        line: {
          animation: false,
          dataLabels: { enabled: false }
        },
        series: { color: '#D1122E' }
      },
      xAxis: {
        labels: {
          style: {
            color: '#FFFFFF'
          }
        },
        type: 'datetime',
        dateTimeLabelFormats: { second: '%S' }
      },
      yAxis: {
        min: 0,
        max: 100,
        labels: {
          style: {
            color: '#FFFFFF'
          }
        },
        title: {
          text: 'mA',
          style: {
            color: '#FFFFFF'
          }
        }
      },
      credits: { enabled: false }
    });
    setInterval(function () {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          var x = (new Date()).getTime(),
            y = parseFloat(this.responseText);
          //console.log(this.responseText);
          if (heaterCurrentChart.series[0].data.length > data_buffer) {
            heaterCurrentChart.series[0].addPoint([x, y], true, true, false);
          } else {
            heaterCurrentChart.series[0].addPoint([x, y], true, false, false);
          }
        }
      };
      xhttp.open("GET", "/htrcurrent", true);
      xhttp.send();
    }, sampling_period_ms);
  }

  showingHeaterChart = !showingHeaterChart;
});


// This part below is for updating just the numeric value of the heater
setInterval(function () {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function () {
    if (this.readyState === 4 && this.status === 200) {
      var x = (new Date()).getTime();
      var y = parseFloat(this.responseText);

      if (showingHeaterChart) {
        // Update the chart
        if (heaterCurrentChart.series[0].data.length > data_buffer) {
          heaterCurrentChart.series[0].addPoint([x, y], true, true, false);
        } else {
          heaterCurrentChart.series[0].addPoint([x, y], true, false, false);
        }
      } else {
        // Update the numeric field
        const outputField = document.getElementById('heaterOutputField');
        if (outputField) {
          outputField.textContent = y.toFixed(2); // Optional: format to 2 decimal places
        }
      }
    }
  };
  xhttp.open("GET", "/htrcurrent", true);
  xhttp.send();
}, slow_sampling_period_ms);

// Here is where I put the toggle slide switch
document.addEventListener("DOMContentLoaded", () => {
  const toggle = document.getElementById("laserToggle"); // ← use the correct ID

  if (!toggle) {
    console.error("Toggle element not found");
    return;
  }

  toggle.addEventListener("change", () => {
    const state = toggle.checked ? 'on' : 'off';
    fetch(`/toggleLaser?state=${state}`)
      .then(res => res.text())
      .then(txt => console.log("ESP32 response:", txt))
      .catch(err => console.error("Error:", err));
  });
});

// Here is where I put the toggle slide switch
document.addEventListener("DOMContentLoaded", () => {
  const toggle = document.getElementById("tecToggle"); // ← use the correct ID

  if (!toggle) {
    console.error("Toggle element not found");
    return;
  }

  toggle.addEventListener("change", () => {
    const state = toggle.checked ? 'on' : 'off';
    fetch(`/toggleTec?state=${state}`)
      .then(res => res.text())
      .then(txt => console.log("ESP32 response:", txt))
      .catch(err => console.error("Error:", err));
  });
});

document.addEventListener("DOMContentLoaded", () => {
  const sendButton = document.getElementById("sendLaserValuesButton");
  const inputField = document.getElementById("laserDACvalue");

  sendButton.addEventListener("click", () => {
    const value = inputField.value;

    if (value === "") {
      alert("Please enter a value.");
      return;
    }

    const numValue = parseFloat(value);
    if (isNaN(numValue) || numValue < 0 || numValue > 255) {
      alert("Please enter a valid number between 0 and 255.");
      return;
    }

    fetch(`/setLaserDAC?value=${numValue}`)
      .then(res => res.text())
      .then(msg => {
        console.log("ESP32 Response:", msg);
      })
      .catch(err => {
        console.error("Error sending value to ESP32:", err);
        alert("Failed to send value.");
      });
  });
});




document.addEventListener("DOMContentLoaded", () => {
  const sendButton = document.getElementById("sendTecValuesButton");
  const inputField = document.getElementById("tecDACvalue");

  sendButton.addEventListener("click", () => {
    const value = inputField.value;

    if (value === "") {
      alert("Please enter a value.");
      return;
    }

    const numValue = parseFloat(value);
    if (isNaN(numValue) || numValue < 0 || numValue > 255) {
      alert("Please enter a valid number between 0 and 255.");
      return;
    }

    fetch(`/setTecDAC?value=${numValue}`)
      .then(res => res.text())
      .then(msg => {
        console.log("ESP32 Response:", msg);
      })
      .catch(err => {
        console.error("Error sending value to ESP32:", err);
        alert("Failed to send value.");
      });
  });
});

function updateLaserFaultLED(status) {
  const laserFaultLED = document.getElementById("laser_fault_led");

  if (status === 1) {
    laserFaultLED.classList.remove("off");
    laserFaultLED.classList.add("on");
  }
  else {
    laserFaultLED.classList.remove("on");
    laserFaultLED.classList.add("off");
  }
}

function updateTecFaultLED(status) {
  const tecFaultLED = document.getElementById("tec_fault_led");

  if (status === 1) {
    tecFaultLED.classList.remove("off");
    tecFaultLED.classList.add("on");
  }
  else {
    tecFaultLED.classList.remove("on");
    tecFaultLED.classList.add("off");
  }
}

function updateLaserOnLED(status) {
  const laserOnLED = document.getElementById("laser_on_led");

  if (status === 1) {
    laserOnLED.classList.remove("off");
    laserOnLED.classList.add("on");
  }
  else {
    laserOnLED.classList.remove("on");
    laserOnLED.classList.add("off");
  }
}

function updateTecOnLED(status) {
  const tecOnLED = document.getElementById("tec_on_led");

  if (status === 1) {
    tecOnLED.classList.remove("off");
    tecOnLED.classList.add("on");
  }
  else {
    tecOnLED.classList.remove("on");
    tecOnLED.classList.add("off");
  }
}

// Poll every second
setInterval(() => {
  fetch("/laserFault")
    .then(response => response.text())
    .then(data => {
      const status = parseInt(data.trim());
      updateLaserFaultLED(status);
    })
    .catch(error => {
      console.error("Error fetching laser fault status:", error);
    });

  fetch("/laserOn")
    .then(response => response.text())
    .then(data => {
      const status = parseInt(data.trim());
      updateLaserOnLED(status);
    })
    .catch(error => {
      console.error("Error fetching laser on status:", error);
    });

}, half_second_sampling);

// Poll every second
setInterval(() => {
  fetch("/tecFault")
    .then(response => response.text())
    .then(data => {
      const status = parseInt(data.trim());
      updateTecFaultLED(status);
    })
    .catch(error => {
      console.error("Error fetching tec fault status:", error);
    });

  fetch("/tecOn")
    .then(response => response.text())
    .then(data => {
      const status = parseInt(data.trim());
      updateTecOnLED(status);
    })
    .catch(error => {
      console.error("Error fetching tec on status:", error);
    });

}, half_second_sampling);


document.getElementById("showLaserGraphButton").addEventListener("click", function () {
  window.open("/laser-graph.html", "_blank", `width=${width},height=${height}`);
});


document.getElementById("showTecGraphButton").addEventListener("click", function () {
  window.open("/tec-graph.html", "_blank", `width=${width / 3},height=${height / 2}`);
});

function scaleToFit() {
  const baseWidth = 1920;
  const baseHeight = 1080;
  const scaleX = window.innerWidth / baseWidth;
  const scaleY = window.innerHeight / baseHeight;
  const scale = Math.min(scaleX, scaleY);

  const wrapper = document.getElementById('wrapper');
  wrapper.style.transform = `scale(${scale})`;
}

window.addEventListener('resize', scaleToFit);
window.addEventListener('load', scaleToFit);

