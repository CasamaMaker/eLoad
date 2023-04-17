setInterval(function() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("ADCA").innerHTML =
        this.responseText;
      }
    };
    xhttp.open("GET", "/readA", true);
    xhttp.send();
  }, 1000);
  
  setInterval(function() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("ADCV").innerHTML =
        this.responseText;
      }
    };
    xhttp.open("GET", "/readV", true);
    xhttp.send();
  }, 1000);
  
  setInterval(function() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("ADCP").innerHTML =
        this.responseText;
      }
    };
    xhttp.open("GET", "/readP", true);
    xhttp.send();
  }, 1000);

  setInterval(function() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("ADCT").innerHTML =
        this.responseText;
      }
    };
    xhttp.open("GET", "/readT", true);
    xhttp.send();
  }, 1000);
  
  
  var chartA = new Highcharts.Chart({
    chart:{ renderTo : 'chart-temperature' },
    title: { text: 'Current' },
    series: [{
      showInLegend: false,
      data: []
    }],
    plotOptions: {
      line: { animation: false,
        dataLabels: { enabled: true }
      },
      series: { color: '#059e8a' }
    },
    xAxis: { type: 'datetime',
      dateTimeLabelFormats: { second: '%H:%M:%S' }
    },
    yAxis: {
      title: { text: 'Ampers [A]' },
      //title: { text: 'Temperature (Fahrenheit)' }
      min: 0,
      max: 5
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
        if(chartA.series[0].data.length > 300) {
          chartA.series[0].addPoint([x, y], true, true, true);
        } else {
          chartA.series[0].addPoint([x, y], true, false, true);
        }
      }
    };
    xhttp.open("GET", "/current", true);
    xhttp.send();
  }, 1000 ) ;
  
  var chartH = new Highcharts.Chart({
    chart:{ renderTo:'chart-humidity' },
    title: { text: 'Tension' },
    series: [{
      showInLegend: false,
      data: []
    }],
    plotOptions: {
      line: { animation: false,
        dataLabels: { enabled: true }
      }
    },
    xAxis: {
      type: 'datetime',
      dateTimeLabelFormats: { second: '%H:%M:%S' }
    },
    yAxis: {
      title: { text: 'Volts [V]' },
      min: 0,
      max: 25
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
        if(chartH.series[0].data.length > 300) {
          chartH.series[0].addPoint([x, y], true, true, true);
        } else {
          chartH.series[0].addPoint([x, y], true, false, true);
        }
      }
    };
    xhttp.open("GET", "/tension", true);
    xhttp.send();
  }, 1000 ) ;
  
  var chartP = new Highcharts.Chart({
    chart:{ renderTo:'chart-pressure' },
    title: { text: 'Power' },
    series: [{
      showInLegend: false,
      data: []
    }],
    plotOptions: {
      line: { animation: false,
        dataLabels: { enabled: true }
      },
      series: { color: '#18009c' }
    },
    xAxis: {
      type: 'datetime',
      dateTimeLabelFormats: { second: '%H:%M:%S' }
    },
    yAxis: {
      title: { text: 'Watts [W]' },
      min: 0,
      max: 60
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
        if(chartP.series[0].data.length > 300) {  //this value is the sample numer in plot
          chartP.series[0].addPoint([x, y], true, true, true);
        } else {
          chartP.series[0].addPoint([x, y], true, false, true);
        }
      }
    };
    xhttp.open("GET", "/power", true);
    xhttp.send();
  }, 1000 ) ; //this value is the sample time

  var chartT = new Highcharts.Chart({
    chart:{ renderTo:'chart-ttt' },
    title: { text: 'Temperature' },
    series: [{
      showInLegend: false,
      data: []
    }],
    plotOptions: {
      line: { animation: false,
        dataLabels: { enabled: true }
      },
      series: { color: '#18009c' }
    },
    xAxis: {
      type: 'datetime',
      dateTimeLabelFormats: { second: '%H:%M:%S' }
    },
    yAxis: {
      title: { text: 'degrees [C]' },
      min: 0,
      max: 120
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
        if(chartT.series[0].data.length > 300) {  //this value is the sample numer in plot
          chartT.series[0].addPoint([x, y], true, true, true);
        } else {
          chartT.series[0].addPoint([x, y], true, false, true);
        }
      }
    };
    xhttp.open("GET", "/temperature", true);
    xhttp.send();
  }, 1000 ) ; //this value is the sample time