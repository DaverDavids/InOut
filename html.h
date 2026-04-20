// html.h — All web UI HTML for InOut sketch
// Included by InOut.ino

#pragma once

// ─── Captive Portal ───────────────────────────────────────────────────────────
const char PAGE_PORTAL[] PROGMEM = R"rawhtml(
<!DOCTYPE html><html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>InOut — WiFi Setup</title>
<style>
  :root{--bg:#f7f6f2;--surface:#fff;--text:#28251d;--muted:#7a7974;--accent:#01696f;--border:#d4d1ca;--r:0.5rem}
  *{box-sizing:border-box;margin:0;padding:0}
  body{min-height:100dvh;display:flex;align-items:center;justify-content:center;
       background:var(--bg);font:16px/1.6 system-ui,sans-serif;color:var(--text)}
  .card{background:var(--surface);border:1px solid var(--border);border-radius:var(--r);
        padding:2rem;width:min(400px,92vw);box-shadow:0 4px 16px #0001}
  h1{font-size:1.25rem;margin-bottom:.25rem}
  p{color:var(--muted);font-size:.875rem;margin-bottom:1.5rem}
  label{display:block;font-size:.875rem;font-weight:500;margin-bottom:.25rem}
  input{width:100%;padding:.5rem .75rem;border:1px solid var(--border);
        border-radius:var(--r);font:inherit;margin-bottom:1rem}
  input:focus{outline:2px solid var(--accent);outline-offset:2px;border-color:transparent}
  button{width:100%;padding:.625rem;background:var(--accent);color:#fff;border:none;
         border-radius:var(--r);font:inherit;font-weight:600;cursor:pointer}
  button:hover{background:#0c4e54}
</style>
</head>
<body>
<div class="card">
  <h1>📡 InOut WiFi Setup</h1>
  <p>Enter your WiFi credentials. Settings are saved to flash.</p>
  <form method="POST" action="/save">
    <label>SSID</label>
    <input name="ssid" type="text" placeholder="Network name" required autocomplete="off">
    <label>Password</label>
    <input name="psk" type="password" placeholder="Password" autocomplete="off">
    <button type="submit">Save &amp; Connect</button>
  </form>
</div>
</body></html>
)rawhtml";

// ─── Main UI ──────────────────────────────────────────────────────────────────
const char PAGE_MAIN[] PROGMEM = R"rawhtml(
<!DOCTYPE html><html lang="en" data-theme="dark">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>InOut</title>
<style>
/* ── tokens ── */
:root,[data-theme="light"]{
  --bg:#f7f6f2;--surface:#ffffff;--surface2:#f3f0ec;--border:#d4d1ca;
  --text:#28251d;--muted:#7a7974;--faint:#bab9b4;
  --accent:#01696f;--accent-h:#0c4e54;
  --success:#437a22;--error:#a12c7b;--warn:#da7101;
  --r:.5rem;--r-sm:.375rem;
  --shadow:0 2px 8px #0001;
}
[data-theme="dark"]{
  --bg:#171614;--surface:#1c1b19;--surface2:#22211f;--border:#393836;
  --text:#cdccca;--muted:#797876;--faint:#5a5957;
  --accent:#4f98a3;--accent-h:#227f8b;
  --success:#6daa45;--error:#d163a7;--warn:#fdab43;
  --shadow:0 2px 8px #0005;
}
*{box-sizing:border-box;margin:0;padding:0}
body{min-height:100dvh;background:var(--bg);font:14px/1.5 system-ui,sans-serif;color:var(--text)}

/* ── layout ── */
header{display:flex;align-items:center;gap:.75rem;padding:.75rem 1rem;
       background:var(--surface);border-bottom:1px solid var(--border);position:sticky;top:0;z-index:10}
header h1{font-size:1rem;font-weight:700;flex:1}
.theme-btn{background:none;border:none;cursor:pointer;color:var(--muted);font-size:1.1rem;padding:.25rem}
.theme-btn:hover{color:var(--text)}
.tab-btn{background:none;border:1px solid var(--border);border-radius:var(--r-sm);padding:.2rem .75rem;color:var(--muted);cursor:pointer;font:inherit;font-size:.8rem}
.tab-btn.active{background:var(--accent);color:#fff;border-color:var(--accent)}
main{max-width:1200px;margin:0 auto;padding:1rem;display:grid;gap:1rem;
     grid-template-columns:1fr 1fr;grid-template-rows:auto 1fr}
@media(max-width:700px){main{grid-template-columns:1fr}}

/* ── pin table ── */
.pin-panel{background:var(--surface);border:1px solid var(--border);border-radius:var(--r);
           overflow:hidden;grid-column:1}
.pin-panel h2,.chart-panel h2{font-size:.875rem;font-weight:600;padding:.625rem 1rem;
                               border-bottom:1px solid var(--border);background:var(--surface2)}
table{width:100%;border-collapse:collapse}
th{font-size:.75rem;font-weight:500;color:var(--muted);text-align:left;
   padding:.4rem .75rem;border-bottom:1px solid var(--border)}
td{padding:.35rem .75rem;border-bottom:1px solid var(--border);font-size:.8125rem;vertical-align:middle}
tr:last-child td{border-bottom:none}
tr:hover td{background:var(--surface2)}

/* ── controls ── */
select,input[type=number]{padding:.2rem .4rem;border:1px solid var(--border);background:var(--bg);
  color:var(--text);border-radius:var(--r-sm);font:inherit;font-size:.75rem;max-width:80px}
select:focus,input[type=number]:focus{outline:2px solid var(--accent);outline-offset:1px}
.btn{display:inline-flex;align-items:center;gap:.25rem;padding:.25rem .6rem;border:1px solid var(--border);
     background:var(--surface2);color:var(--text);border-radius:var(--r-sm);
     font:inherit;font-size:.75rem;cursor:pointer;transition:background .15s}
.btn:hover{background:var(--border)}
.btn-accent{background:var(--accent);color:#fff;border-color:var(--accent)}
.btn-accent:hover{background:var(--accent-h);border-color:var(--accent-h)}
.dir-badge{display:inline-block;padding:.1rem .4rem;border-radius:var(--r-sm);font-size:.7rem;font-weight:600}
.dir-in {background:color-mix(in oklch,var(--accent) 15%,var(--surface));color:var(--accent)}
.dir-out{background:color-mix(in oklch,var(--warn) 15%,var(--surface));color:var(--warn)}
.val-badge{display:inline-block;width:20px;height:20px;border-radius:50%;
           background:var(--faint);border:2px solid var(--border)}
.val-badge.hi{background:var(--success);border-color:var(--success)}
.mon-cb{width:16px;height:16px;accent-color:var(--accent);cursor:pointer}

/* ── chart panel ── */
.chart-panel{background:var(--surface);border:1px solid var(--border);border-radius:var(--r);
             overflow:hidden;grid-column:2;display:flex;flex-direction:column}
@media(max-width:700px){.chart-panel{grid-column:1}}
.chart-panel canvas{flex:1;min-height:0;padding:.5rem}
.chart-legend{display:flex;flex-wrap:wrap;gap:.5rem;padding:.5rem 1rem;border-top:1px solid var(--border)}
.legend-item{display:flex;align-items:center;gap:.3rem;font-size:.75rem;color:var(--muted)}
.legend-dot{width:10px;height:10px;border-radius:50%;flex-shrink:0}

/* ── status bar ── */
.status{grid-column:1/-1;font-size:.75rem;color:var(--muted);
        padding:.4rem .75rem;background:var(--surface);border:1px solid var(--border);
        border-radius:var(--r);display:flex;align-items:center;gap:.5rem}
.dot{width:8px;height:8px;border-radius:50%;background:var(--faint);flex-shrink:0}
.dot.ok{background:var(--success)}
</style>
</head>
<body>
<header>
  <svg width="22" height="22" viewBox="0 0 24 24" fill="none" stroke="currentColor"
       stroke-width="2" stroke-linecap="round" stroke-linejoin="round" aria-hidden="true">
    <circle cx="12" cy="12" r="3"/>
    <path d="M3 9h2M19 9h2M3 15h2M19 15h2M9 3v2M15 3v2M9 19v2M15 19v2"/>
  </svg>
  <h1>InOut</h1>
  <button class="tab-btn active" onclick="showPage('gpio')" id="tabGpio">GPIO</button>
  <button class="tab-btn" onclick="showPage('wifi')" id="tabWifi">WiFi</button>
  <span id="ip" style="font-size:.75rem;color:var(--muted)"></span>
  <button class="theme-btn" onclick="toggleTheme()" title="Toggle theme" aria-label="Toggle theme">🌓</button>
</header>

<div id="pageGpio">
<main>
  <!-- Status -->
  <div class="status" style="grid-row:1">
    <div class="dot ok" id="connDot"></div>
    <span id="connLabel">Polling…</span>
  </div>

  <!-- Pin table -->
  <div class="pin-panel" style="grid-row:2">
    <h2>GPIO Pins</h2>
    <table>
      <thead>
        <tr>
          <th>GPIO</th><th>Dir</th><th>Value</th><th>Set</th><th>Graph</th>
        </tr>
      </thead>
      <tbody id="pinTable"></tbody>
    </table>
  </div>

  <!-- Chart -->
  <div class="chart-panel" style="grid-row:2">
    <h2>Live Graph</h2>
    <canvas id="chart"></canvas>
    <div class="chart-legend" id="legend"></div>
  </div>
</main>
</div>

<div id="pageWifi" style="display:none;max-width:700px;margin:1rem auto;overflow:hidden">
  <div style="background:var(--surface);border:1px solid var(--border);border-radius:var(--r);padding:1.5rem;text-align:center;margin-bottom:1rem">
    <div style="font-size:.875rem;color:var(--muted);margin-bottom:.5rem">Current RSSI</div>
    <div id="wifiRssi" style="font-size:4rem;font-weight:900;letter-spacing:-2px;color:var(--accent)">--</div>
    <div style="font-size:1rem;color:var(--muted)">dBm</div>
    <div style="margin-top:1.5rem;font-size:.875rem;color:var(--muted)">5-Second Average</div>
    <div id="wifiAvg" style="font-size:3rem;font-weight:800;color:var(--warn)">--</div>
    <div style="font-size:1rem;color:var(--muted)">dBm</div>
  </div>
  <div class="chart-panel" style="display:block">
    <div style="font-size:.875rem;font-weight:600;padding:.625rem 1rem;border-bottom:1px solid var(--border);background:var(--surface2);border-radius:var(--r) var(--r) 0 0">Signal History</div>
    <canvas id="wifiChart" style="width:100%;height:220px;min-height:0"></canvas>
  </div>
</div>

<script src="https://cdn.jsdelivr.net/npm/chart.js@4/dist/chart.umd.min.js"></script>
<script>
/* ── theme ── */
const root = document.documentElement;
function toggleTheme(){
  root.dataset.theme = root.dataset.theme === 'dark' ? 'light' : 'dark';
}

/* ── chart ── */
const PALETTE = ['#4f98a3','#fdab43','#6daa45','#d163a7','#5591c7','#e8af34','#bb653b','#a86fdf'];
const MAX_PTS = 60;
const ctx = document.getElementById('chart').getContext('2d');

const chartData = {
  labels: [],
  datasets: []
};
const chart = new Chart(ctx, {
  type: 'line',
  data: chartData,
  options: {
    responsive: true,
    maintainAspectRatio: false,
    animation: {duration: 0},
    scales: {
      x: {display: false},
      y: {min: -0.1, max: 1.1,
          ticks: {
            color: getComputedStyle(root).getPropertyValue('--muted').trim(),
            font: {size: 11}
          },
          grid: {color: getComputedStyle(root).getPropertyValue('--border').trim()}
      }
    },
    plugins: {
      legend: {display: false},
      tooltip: {
        callbacks: {
          label: ctx => `GPIO${ctx.dataset.label}: ${ctx.parsed.y}`
        }
      }
    }
  }
});

/* ── pin state cache ── */
let pins = [];

function renderTable(data) {
  pins = data;
  const tbody = document.getElementById('pinTable');
  tbody.innerHTML = '';
  data.forEach(p => {
    const row = document.createElement('tr');
    row.innerHTML = `
      <td><strong>GPIO${p.pin}</strong></td>
      <td>
        <select onchange="setDir(${p.id},this.value)" title="Direction">
          <option value="0" ${p.dir==0?'selected':''}>IN</option>
          <option value="1" ${p.dir==1?'selected':''}>OUT</option>
        </select>
      </td>
      <td>
        <span class="val-badge ${p.val?'hi':''}" title="${p.val?'HIGH':'LOW'}"></span>
        <span style="margin-left:.25rem;color:var(--muted);font-size:.7rem">${p.val?'HI':'LO'}</span>
      </td>
      <td>
        ${p.dir==1 ? `
          <button class="btn btn-accent" onclick="setVal(${p.id},1)" title="Set HIGH">▲</button>
          <button class="btn" onclick="setVal(${p.id},0)" title="Set LOW">▼</button>
        ` : '<span style="color:var(--faint);font-size:.7rem">read-only</span>'}
      </td>
      <td>
        <input type="checkbox" class="mon-cb" ${p.mon?'checked':''}
               onchange="setMon(${p.id},this.checked)" title="Monitor on graph">
      </td>
    `;
    tbody.appendChild(row);
  });
  updateChartDatasets(data);
  renderLegend(data);
}

/* ── chart dataset management ── */
function updateChartDatasets(data) {
  const monitored = data.filter(p => p.mon);
  // Add missing datasets
  monitored.forEach((p, ci) => {
    const lbl = String(p.pin);
    if (!chart.data.datasets.find(d => d.label === lbl)) {
      chart.data.datasets.push({
        label: lbl,
        data: [],
        borderColor: PALETTE[ci % PALETTE.length],
        backgroundColor: 'transparent',
        borderWidth: 2,
        pointRadius: 0,
        tension: 0.3
      });
    }
  });
  // Remove dropped datasets
  chart.data.datasets = chart.data.datasets.filter(
    d => monitored.some(p => String(p.pin) === d.label)
  );
}

function pushChartPoint(data) {
  const ts = new Date().toLocaleTimeString();
  chart.data.labels.push(ts);
  if (chart.data.labels.length > MAX_PTS) chart.data.labels.shift();

  data.filter(p => p.mon).forEach(p => {
    const ds = chart.data.datasets.find(d => d.label === String(p.pin));
    if (ds) {
      ds.data.push(p.val);
      if (ds.data.length > MAX_PTS) ds.data.shift();
    }
  });
  chart.update('none');
}

function renderLegend(data) {
  const el = document.getElementById('legend');
  el.innerHTML = '';
  data.filter(p => p.mon).forEach((p, ci) => {
    const col = PALETTE[ci % PALETTE.length];
    el.innerHTML += `
      <div class="legend-item">
        <div class="legend-dot" style="background:${col}"></div>
        GPIO${p.pin}
      </div>`;
  });
  if (!data.some(p => p.mon)) {
    el.innerHTML = '<span style="color:var(--faint)">No pins selected for graphing</span>';
  }
}

/* ── API calls ── */
async function post(params) {
  const body = new URLSearchParams(params);
  const r = await fetch('/pin', {method:'POST', body});
  const data = await r.json();
  renderTable(data);
}
function setDir(id, val) { post({id, dir: val}); }
function setVal(id, val) { post({id, val}); }
function setMon(id, checked) { post({id, mon: checked ? 1 : 0}); }

/* ── polling ── */
async function poll() {
  try {
    const r = await fetch('/pins');
    if (!r.ok) throw new Error();
    const data = await r.json();
    renderTable(data);
    pushChartPoint(data);
    document.getElementById('connDot').className = 'dot ok';
    document.getElementById('connLabel').textContent = 'Connected · ' + new Date().toLocaleTimeString();
  } catch {
    document.getElementById('connDot').className = 'dot';
    document.getElementById('connLabel').textContent = 'Disconnected — retrying…';
  }
  setTimeout(poll, 500);
}

poll();

/* ── WiFi page ── */
const wifiRssiHistory = [];
const WIFI_MAX_PTS = 60;
const WIFI_AVG_WINDOW = 10;

const wctx = document.getElementById('wifiChart').getContext('2d');
const wifiChartData = { labels: [], datasets: [
  { label: 'RSSI', data: [], borderColor: '#4f98a3', backgroundColor: 'transparent', borderWidth: 2, pointRadius: 0, tension: 0.3 },
  { label: '5s Avg', data: [], borderColor: '#fdab43', backgroundColor: 'transparent', borderWidth: 2, borderDash: [5,3], pointRadius: 0, tension: 0.3 }
]};
const wifiChart = new Chart(wctx, {
  type: 'line', data: wifiChartData,
  options: {
    responsive: true, maintainAspectRatio: false, animation: {duration:0},
    scales: {
      x: {display: false},
      y: { min: -100, max: -20, ticks: { color: '#888', callback: v => v + ' dBm' }, grid: {color: '#333'} }
    },
    plugins: { legend: { display: true } }
  }
});

async function pollWifi() {
  if (document.getElementById('pageWifi').style.display !== 'none') {
    try {
      const r = await fetch('/wifi');
      const d = await r.json();
      const rssi = d.rssi;
      wifiRssiHistory.push(rssi);
      if (wifiRssiHistory.length > WIFI_MAX_PTS) wifiRssiHistory.shift();

      const slice = wifiRssiHistory.slice(-WIFI_AVG_WINDOW);
      const avg = Math.round(slice.reduce((a,b) => a+b, 0) / slice.length);

      document.getElementById('wifiRssi').textContent = rssi;
      document.getElementById('wifiAvg').textContent = avg;

      const ts = new Date().toLocaleTimeString();
      wifiChartData.labels.push(ts);
      if (wifiChartData.labels.length > WIFI_MAX_PTS) wifiChartData.labels.shift();
      wifiChartData.datasets[0].data.push(rssi);
      if (wifiChartData.datasets[0].data.length > WIFI_MAX_PTS) wifiChartData.datasets[0].data.shift();
      wifiChartData.datasets[1].data.push(avg);
      if (wifiChartData.datasets[1].data.length > WIFI_MAX_PTS) wifiChartData.datasets[1].data.shift();
      wifiChart.update('none');
    } catch(e) {}
  }
  setTimeout(pollWifi, 500);
}

function showPage(name) {
  document.getElementById('pageGpio').style.display = name === 'gpio' ? '' : 'none';
  document.getElementById('pageWifi').style.display = name === 'wifi' ? '' : 'none';
  document.getElementById('tabGpio').classList.toggle('active', name === 'gpio');
  document.getElementById('tabWifi').classList.toggle('active', name === 'wifi');
}

showPage('gpio');
pollWifi();
</script>
</body></html>
)rawhtml";
