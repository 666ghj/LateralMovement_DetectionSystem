<template>
  <div class="d-flex">
    <div class="input-group">
      <span class="input-group-text"><i class="bi bi-hdd-network"></i></span>
      <input class="form-control flex-grow-1" placeholder="备份服务器 HTTP 地址" v-model="input">
    </div>
    <button type="button" class="btn btn-outline-primary ms-2 flex-shrink-0" @click="set()">
      <i v-if="!isSetting" class="bi bi-gear"></i>
      <span v-if="isSetting" class="spinner-border spinner-border-sm" role="status" aria-hidden="true"></span>
      设置
    </button>
  </div>
  <div class="mt-2">
    <div class="small text-secondary">
      当前备份服务器：
      <a :href="server" target="_blank">{{ server }}</a>
    </div>
  </div>
</template>

<script>
export default {
  name: 'BackupSettings',
  mounted() {
    fetch('/api/server', {
      method: 'GET',
      headers: {
        'Content-Type': 'application/json',
      },
    })
      .then((response) => {
        if (response.ok) {
          return response.json();
        } else {
          throw 'URL 错误';
        }
      })
      .then((data) => {
        this.server = data.server;
      })
      .catch((error) => {
        alert(`错误：${error}`);
        console.error('Error:', error);
      });
  },
  data() {
    return {
      input: '',
      server: '',
      isSetting: false,
    }
  },
  methods: {
    async set() {
      this.isSetting = true;
      await fetch('/api/server', {
        method: 'PUT',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({
          server: this.input
        })
      })
        .then((response) => {
          if (response.ok) {
            return response.json();
          } else {
            throw 'URL 错误或服务器不可用';
          }
        })
        .then((data) => {
          this.server = data.server;
        })
        .catch((error) => {
          alert(`错误：${error}`);
          console.error('Error:', error);
        });
      this.input = '';
      this.isSetting = false;
      window.location.reload();
    }
  }
}
</script>
