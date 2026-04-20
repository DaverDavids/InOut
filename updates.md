<img src="https://r2cdn.perplexity.ai/pplx-full-logo-primary-dark%402x.png" style="height:64px;margin-right:32px"/>

# look at github repo InOut.... add a wifi page where we can see signal strength to the ap, and also an average over 5 seconds, both in very visible letters, and graph them both. Describe all changes needed to me precisely, but very concisely, tell me exactly what to change

Here's a precise, concise breakdown of every change needed:

***

## `InOut.ino` — Add `/wifi` API endpoint

In `setupRoutes()`, add this new route after the `/pins` route :

```cpp
server.on("/wifi", HTTP_GET, []() {
  int rssi = WiFi.RSSI();
  String j = "{\"rssi\":" + String(rssi) + ",\"ssid\":\"" + String(savedSSID) + "\"}";
  server.send(200, "application/json", j);
});
```

No other changes needed in `InOut.ino`.

***

## `html.h` — Add WiFi page

### 1. Add a nav tab to the `<header>`

```
After `<h1>InOut</h1>`, add two nav buttons :
```

```html
<button class="tab-btn" onclick="showPage('gpio')" id="tabGpio">GPIO</button>
<button class="tab-btn" onclick="showPage('wifi')" id="tabWifi">WiFi</button>
```

Add to `<style>`:

```css
.tab-btn{background:none;border:1px solid var(--border);border-radius:var(--r-sm);
         padding:.2rem .75rem;color:var(--muted);cursor:pointer;font:inherit;font-size:.8rem}
.tab-btn.active{background:var(--accent);color:#fff;border-color:var(--accent)}
```


***

### 2. Wrap existing `<main>` in a div

Rename/wrap existing `<main>` tag:

```html
<div id="pageGpio"> ... existing <main>...</main> ... </div>
```


***

### 3. Add the WiFi page div (after `pageGpio`)

```html
<div id="pageWifi" style="display:none;max-width:700px;margin:1rem auto;display:none">
  <div style="background:var(--surface);border:1px solid var(--border);border-radius:var(--r);padding:1.5rem;text-align:center;margin-bottom:1rem">
    <div style="font-size:.875rem;color:var(--muted);margin-bottom:.5rem">Current RSSI</div>
    <div id="wifiRssi" style="font-size:4rem;font-weight:900;letter-spacing:-2px;color:var(--accent)">--</div>
    <div style="font-size:1rem;color:var(--muted)">dBm</div>
    <div style="margin-top:1.5rem;font-size:.875rem;color:var(--muted)">5-Second Average</div>
    <div id="wifiAvg" style="font-size:3rem;font-weight:800;color:var(--warn)">--</div>
    <div style="font-size:1rem;color:var(--muted)">dBm</div>
  </div>
  <div style="background:var(--surface);border:1px solid var(--border);border-radius:var(--r);padding:.75rem">
    <div style="font-size:.875rem;font-weight:600;margin-bottom:.5rem">Signal History</div>
    <canvas id="wifiChart" style="width:100%;height:220px"></canvas>
  </div>
</div>
```


***

### 4. Add WiFi JS (inside `<script>`, at the bottom)

```js
/* ── WiFi page ── */
const wifiRssiHistory = [];
const WIFI_MAX_PTS = 60;
const WIFI_AVG_WINDOW = 10; // 10 polls × 500ms = 5s

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
      y: { ticks: { color: '#888', callback: v => v + ' dBm' }, grid: {color: '#333'} }
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
```


***

## Summary of all files touched

| File | Change |
| :-- | :-- |
| `InOut.ino` | Add `/wifi` GET route returning `{"rssi":-NN,"ssid":"..."}` |
| `html.h` | Add tab nav, WiFi page div with big RSSI + avg readouts, Chart.js graph showing both lines, and JS polling logic |

The `/wifi` endpoint returns live RSSI via `WiFi.RSSI()` . The JS polls it every 500ms (same as GPIO), keeps a 60-point rolling buffer, and computes the 5-second average over the last 10 samples . Both the instant and average values render in large bold text, and both are plotted as separate lines on the chart.

