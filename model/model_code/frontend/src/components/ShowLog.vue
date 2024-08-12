<template>
  <div class="card mt-3" style="min-height: 400px; max-height: 600px; overflow-y: auto;">
    <pre class="card-body">{{ this.log }}</pre>
  </div>
</template>

<script>
import io from 'socket.io-client';

export default {
  name: 'ShowLog',
  data() {
    return {
      log: "",
      socket: null
    }
  },
  mounted() {
    this.socket = io(window.location.origin);
    this.socket.on('log', (msg) => {
      let now = new Date();
      let localTime = now.toLocaleString();
      this.log = `[${localTime}] ${msg}\n${this.log}`;
    });
  }
}
</script>
