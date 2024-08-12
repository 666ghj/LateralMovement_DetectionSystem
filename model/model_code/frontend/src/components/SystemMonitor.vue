<template>
  <div class="system-monitor">
    <div class="chart-container">
      <h3>CPU 使用情况</h3>
      <LineChart :chart-data="cpuData" :options="chartOptions"></LineChart>
    </div>
    <div class="chart-container">
      <h3>内存 使用情况</h3>
      <LineChart :chart-data="memoryData" :options="chartOptions"></LineChart>
    </div>
  </div>
</template>

<script>
import { Line } from 'vue-chartjs'
import { Chart as ChartJS, Title, Tooltip, Legend, LineElement, PointElement, CategoryScale, LinearScale } from 'chart.js'

ChartJS.register(Title, Tooltip, Legend, LineElement, PointElement, CategoryScale, LinearScale)

const LineChart = {
  extends: Line,
  props: ['chartData', 'options'],
  watch: {
    chartData: {
      handler(newData) {
        this.renderChart(newData, this.options)
      },
      deep: true
    }
  },
  mounted() {
    this.renderChart(this.chartData, this.options)
  }
}

export default {
  name: 'SystemMonitor',
  components: {
    LineChart
  },
  data() {
    return {
      cpuData: {
        labels: [],
        datasets: [
          {
            label: 'CPU 使用率 (%)',
            backgroundColor: 'rgba(54, 162, 235, 0.2)',
            borderColor: 'rgba(54, 162, 235, 1)',
            borderWidth: 1,
            data: []
          }
        ]
      },
      memoryData: {
        labels: [],
        datasets: [
          {
            label: '内存 使用率 (%)',
            backgroundColor: 'rgba(255, 99, 132, 0.2)',
            borderColor: 'rgba(255, 99, 132, 1)',
            borderWidth: 1,
            data: []
          }
        ]
      },
      chartOptions: {
        responsive: true,
        maintainAspectRatio: false,
        scales: {
          y: {
            beginAtZero: true,
            max: 100
          }
        }
      }
    }
  },
  mounted() {
    this.updateSystemInfo()
    setInterval(this.updateSystemInfo, 1000)
  },
  methods: {
    updateSystemInfo() {
      // 这里使用伪数据来模拟CPU和内存使用情况
      const fakeCpuLoad = Math.random() * 100
      const fakeMemoryUsage = Math.random() * 100

      this.addData(this.cpuData, fakeCpuLoad.toFixed(2))
      this.addData(this.memoryData, fakeMemoryUsage.toFixed(2))
    },
    addData(chartData, value) {
      const currentTime = new Date().toLocaleTimeString()
      if (chartData.labels.length === 10) {
        chartData.labels.shift()
        chartData.datasets[0].data.shift()
      }
      chartData.labels.push(currentTime)
      chartData.datasets[0].data.push(value)
    }
  }
}
</script>

<style scoped>
.system-monitor {
  display: flex;
  flex-direction: column;
  gap: 20px;
  width: 100%;
}

.chart-container {
  width: 100%;
  height: 300px; /* 可根据需要调整高度 */
}
</style>
