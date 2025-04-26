let chart;

async function fetchData() {
  const res = await fetch('/get-data');
  const data = await res.json();

  const labels = data.map(d => d.timestamp);
  const rain = data.map(d => d.rain);
  const light = data.map(d => d.light);

  document.getElementById("rainValue").textContent = rain.at(-1);
  document.getElementById("lightValue").textContent = light.at(-1);
  document.getElementById("roofStatus").textContent = data.at(-1).roof;

  if (!chart) {
    const ctx = document.getElementById('myChart').getContext('2d');
    chart = new Chart(ctx, {
      type: 'line',
      data: {
        labels,
        datasets: [
          { label: 'Mưa', data: rain, borderColor: 'blue', fill: false },
          { label: 'Ánh sáng', data: light, borderColor: 'orange', fill: false }
        ]
      },
      options: {
        responsive: true,
        animation: false,
        scales: {
          y: { beginAtZero: true }
        }
      }
    });
  } else {
    chart.data.labels = labels;
    chart.data.datasets[0].data = rain;
    chart.data.datasets[1].data = light;
    chart.update();
  }
}

setInterval(fetchData, 1000);
fetchData();

async function sendCommand(action) {
  // Gửi lệnh mở/đóng mái che đến server Flask
  const res = await fetch('/control', {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json',
    },
    body: JSON.stringify({ command: action }),
  });

  const data = await res.json();
  if (data.status === 'command_sent') {
    alert("Lệnh " + action + " đã được gửi!");
  }
}
