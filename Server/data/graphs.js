var sampling_period = 100;
var data_buffer = 100;
var laserDacChart = new Highcharts.Chart({
chart:{ 
    renderTo : 'LASER DAC',
    backgroundColor: '#4A4A4A'
},
title: {
    text: 'LASER',
    style: {
        color: '#FFFFFF'
      }
 },
series: [{
  showInLegend: false,
  data: []
}],
plotOptions: {
  line: { animation: false,
    dataLabels: { enabled: false }
  },
  series: { color: '#00d78c' },
  style: {
    color: '#FFFFFF'
  }
},
xAxis: { 
    labels: {
        style: {
        color: '#FFFFFF'
        }
    },
    type: 'datetime',
    dateTimeLabelFormats: { second: '%S' },
},
yAxis: {
    labels: {
        style: {
          color: '#FFFFFF'
        }
    },
    title: {
        text: 'DAC VALUE',
        style: {
            color: '#FFFFFF'
        }
    },
},
credits: { enabled: false }
});
setInterval(function ( ) {
var xhttp = new XMLHttpRequest();
xhttp.onreadystatechange = function() {
  if (this.readyState == 4 && this.status == 200) {
    var x = (new Date()).getTime(),
        y = parseFloat(this.responseText);
    //console.log(this.responseText);
    if(laserDacChart.series[0].data.length > data_buffer) {
      laserDacChart.series[0].addPoint([x, y], true, true, true);
    } else {
      laserDacChart.series[0].addPoint([x, y], true, false, true);
    }
  }
};
xhttp.open("GET", "/laserdac", true);
xhttp.send();
}, sampling_period ) ;

var laserCurrentChart = new Highcharts.Chart({
chart:{ 
    renderTo:'LASER Current',
    backgroundColor: '#4A4A4A'
},
title: {
    text: 'Laser Current',
    style: {
        color: '#FFFFFF'
    } },
series: [{
  showInLegend: false,
  data: []
}],
plotOptions: {
  line: { animation: false,
    dataLabels: { enabled: false }
  },
  series: {color: '#D1122E'}
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
    labels: {
        style: {
          color: '#FFFFFF'
        }
      },
    title: {
        text: 'Laser Current',
        style: {
            color: '#FFFFFF'
        }
    }
},
credits: { enabled: false }
});
setInterval(function ( ) {
var xhttp = new XMLHttpRequest();
xhttp.onreadystatechange = function() {
  if (this.readyState == 4 && this.status == 200) {
    var x = (new Date()).getTime(),
        y = parseFloat(this.responseText);
    //console.log(this.responseText);
    if(laserCurrentChart.series[0].data.length > data_buffer) {
      laserCurrentChart.series[0].addPoint([x, y], true, true, true);
    } else {
      laserCurrentChart.series[0].addPoint([x, y], true, false, true);
    }
  }
};
xhttp.open("GET", "/lasercurrent", true);
xhttp.send();
}, sampling_period ) ;

var tecDacChart = new Highcharts.Chart({
chart:{ 
    renderTo:'TEC DAC',
    backgroundColor: '#4A4A4A' 
},
title: {
    text: 'TEC DAC',
    style: {
        color: '#FFFFFF'
    }},
series: [{
  showInLegend: false,
  data: []
}],
plotOptions: {
  line: { animation: false,
    dataLabels: { enabled: false }
  },
  series: { color: '#18009c' }
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
    labels: {
        style: {
          color: '#FFFFFF'
        }
    },
    title: {
        text: 'TEC DAC value',
        style: {
            color: '#FFFFFF'
        }
    }
},
credits: { enabled: false }
});
setInterval(function ( ) {
var xhttp = new XMLHttpRequest();
xhttp.onreadystatechange = function() {
  if (this.readyState == 4 && this.status == 200) {
    var x = (new Date()).getTime(),
        y = parseFloat(this.responseText);
    //console.log(this.responseText);
    if(tecDacChart.series[0].data.length > data_buffer) {
      tecDacChart.series[0].addPoint([x, y], true, true, true);
    } else {
      tecDacChart.series[0].addPoint([x, y], true, false, true);
    }
  }
};
xhttp.open("GET", "/tecdac", true);
xhttp.send();
}, sampling_period ) ;

var tecCurrentChart = new Highcharts.Chart({
chart:{ 
    renderTo:'TEC Current',
    backgroundColor: '#4A4A4A'
},
title: {
    text: 'TEC Current',
    style: {
        color: '#FFFFFF'
    }},
series: [{
  showInLegend: false,
  data: []
}],
plotOptions: {
  line: { animation: false,
    dataLabels: { enabled: false }
  },
  series: { color: '#18009c' }
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
    labels: {
        style: {
          color: '#FFFFFF'
        }
    },
    title: {
        text: 'TEC current value',
        style: {
            color: '#FFFFFF'
        }
    }
},
credits: { enabled: false }
});
setInterval(function ( ) {
var xhttp = new XMLHttpRequest();
xhttp.onreadystatechange = function() {
  if (this.readyState == 4 && this.status == 200) {
    var x = (new Date()).getTime(),
        y = parseFloat(this.responseText);
    //console.log(this.responseText);
    if(tecCurrentChart.series[0].data.length > data_buffer) {
      tecCurrentChart.series[0].addPoint([x, y], true, true, true);
    } else {
      tecCurrentChart.series[0].addPoint([x, y], true, false, true);
    }
  }
};
xhttp.open("GET", "/teccurrent", true);
xhttp.send();
}, sampling_period ) ;

var tecTemperatureChart = new Highcharts.Chart({
chart:{ 
    renderTo:'TEC Temperature',
    backgroundColor: '#4A4A4A'
},
title: {
    text: 'TEC Temperature',
    style: {
        color: '#FFFFFF'
    }},
series: [{
  showInLegend: false,
  data: []
}],
plotOptions: {
  line: { animation: false,
    dataLabels: { enabled: false }
  },
  series: { color: '#18009c' }
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
setInterval(function ( ) {
var xhttp = new XMLHttpRequest();
xhttp.onreadystatechange = function() {
  if (this.readyState == 4 && this.status == 200) {
    var x = (new Date()).getTime(),
        y = parseFloat(this.responseText);
    //console.log(this.responseText);
    if(tecTemperatureChart.series[0].data.length > data_buffer) {
      tecTemperatureChart.series[0].addPoint([x, y], true, true, true);
    } else {
      tecTemperatureChart.series[0].addPoint([x, y], true, false, true);
    }
  }
};
xhttp.open("GET", "/tectemperature", true);
xhttp.send();
}, sampling_period ) ;

var heaterCurrentChart = new Highcharts.Chart({
chart:{
    renderTo:'Heater Current',
    backgroundColor: '#4A4A4A'
},
title: {
    text: 'Heater Current',
    style: {
        color: '#FFFFFF'
    }},
series: [{
  showInLegend: false,
  data: []
}],
plotOptions: {
  line: { animation: false,
    dataLabels: { enabled: false }
  },
  series: { color: '#18009c' }
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
    labels: {
        style: {
          color: '#FFFFFF'
        }
    },
    title: {
        text: 'Heater Current',
        style: {
            color: '#FFFFFF'
        }
    }
},
credits: { enabled: false }
});
setInterval(function ( ) {
var xhttp = new XMLHttpRequest();
xhttp.onreadystatechange = function() {
  if (this.readyState == 4 && this.status == 200) {
    var x = (new Date()).getTime(),
        y = parseFloat(this.responseText);
    //console.log(this.responseText);
    if(heaterCurrentChart.series[0].data.length > data_buffer) {
      heaterCurrentChart.series[0].addPoint([x, y], true, true, true);
    } else {
      heaterCurrentChart.series[0].addPoint([x, y], true, false, true);
    }
  }
};
xhttp.open("GET", "/htrcurrent", true);
xhttp.send();
}, sampling_period ) ;


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
  
  