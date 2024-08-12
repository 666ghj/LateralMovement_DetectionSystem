<template>
  <div class="fixed-sidebar">
    <div class="card">
      <div class="card-header">
        基本信息
      </div>
<!--      <div class="containerTime">-->
<!--        <TimeClock></TimeClock>-->
<!--      </div>-->
      <div class="card-body">
        <div class="content-center">
          <p>当前时间：{{ currentTime }}</p>
          <p>当前日期：{{ currentDate }}</p>
          <p>电脑IP：{{ ip }}</p>
        </div>
      </div>

    </div>
<!--    <SystemMonitor></SystemMonitor>-->
  </div>
</template>

<script>
// import TimeClock from "@/components/TimeClock.vue";
// import SystemMonitor from "@/components/SystemMonitor.vue";

export default {

  name: 'FixedSidebar',
  // components: {SystemMonitor},
  // components: {TimeClock},
  // components: {TimeClock},
  data() {
    return {
      currentTime: new Date().toLocaleTimeString(),
      currentDate: new Date().toLocaleDateString(),
      ip: '正在获取IP...'
    }
  },
  mounted() {
    this.updateTime();
    this.getIp();
  },
  methods: {
    updateTime() {
      setInterval(() => {
        this.currentTime = new Date().toLocaleTimeString();
      }, 1000);
    },
    getIp() {
      fetch('https://api.ipify.org?format=json')
        .then(response => response.json())
        .then(data => {
          this.ip = data.ip;
        })
        .catch(() => {
          this.ip = '无法获取IP';
        });
    }
  }
}
</script>

<style scoped>
.fixed-sidebar {
  position: fixed;
  top: 10%;
  right: 20px;
  z-index: 1030; /* 确保侧边栏在其他内容之上 */
  width: 250px;
}
.card {
  box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
  border-radius: 8px;
  overflow: hidden;
}
.card-header {
  background-color: #007bff;
  color: white;
  padding: 10px;
  font-size: 1.25rem;
  text-align: center;
}
.card-body {
  padding: 15px;
  display: flex;
  align-items: center;
  justify-content: center;
  height: 170px; /* 可以根据需要调整 */
}
.content-center {
  text-align: center;
}

</style>
