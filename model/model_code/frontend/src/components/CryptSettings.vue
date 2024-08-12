<template>
  <div class="input-group">
    <span class="input-group-text"><i class="bi bi-window"></i></span>
    <input class="form-control flex-grow-1" placeholder="请输入加密进程路径" v-model="pathPrefix"
      :disabled="isLoaded || isLoading">
  </div>
  <div class="input-group mt-2">
    <span class="input-group-text"><i class="bi bi-key"></i></span>
    <input class="form-control" placeholder="请输入加密密码" v-model="password" :disabled="isLoaded || isLoading">
  </div>
  <div class="d-flex justify-content-between align-items-center mt-2">
    <div>
      <div v-if="isLoaded" class="small text-primary">
        <i class="bi bi-check-circle"></i> 驱动已加载
      </div>
      <div v-else-if="isLoading" class="small text-secondary">
        <span class="spinner-grow spinner-grow-sm"></span>
      </div>
      <div v-else class="small text-secondary">
        <i class="bi bi-exclamation-circle"></i> 驱动未加载
      </div>
    </div>
    <div>
      <button type="button" class="btn btn-success ms-2" :disabled="isLoaded || isLoading" @click="load()">
        <i v-if="!isLoading" class="bi bi-lock-fill"></i>
        <span v-if="isLoading" class="spinner-border spinner-border-sm" role="status" aria-hidden="true"></span>
        加载驱动
      </button>
      <button type="button" class="btn btn-danger ms-2" :disabled="!isLoaded || isUnloading" @click="unload()">
        <i v-if="!isUnloading" class="bi bi-unlock-fill"></i>
        <span v-if="isUnloading" class="spinner-border spinner-border-sm" role="status" aria-hidden="true"></span>
        卸载驱动
      </button>
    </div>
  </div>
</template>

<script>
export default {
  name: 'CryptSettings',
  mounted() {
    fetch('/api/status', {
      method: 'GET',
      headers: {
        'Content-Type': 'application/json',
      }
    })
      .then((response) => {
        if (response.ok) {
          return response.json();
        } else {
          throw '内部异常';
        }
      })
      .then((data) => {
        this.pathPrefix = data.pathPrefix;
        this.password = data.password;
        this.isLoaded = data.isLoaded;
      })
      .catch((error) => {
        alert(`错误：${error}`);
        console.error('Error:', error);
      });
  },
  data() {
    return {
      pathPrefix: '',
      password: '',
      isLoaded: false,
      isLoading: false,
      isUnloading: false
    }
  },
  methods: {
    async load() {
      if (this.pathPrefix === '' || this.password === '') {
        alert('请填写加密进程路径和密码！');
        return;
      }
      this.isLoading = true;
      await fetch('/api/load', {
        method: 'PUT',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({
          pathPrefix: this.pathPrefix,
          password: this.password
        })
      })
        .then((response) => {
          if (response.ok) {
            this.isLoaded = true;
          } else {
            throw '加载失败';
          }
        })
        .catch((error) => {
          alert(`错误：${error}`);
          console.error('Error:', error);
        });
      this.isLoading = false;
    },

    async unload() {
      this.isUnloading = true;
      await fetch('/api/unload', {
        method: 'GET',
        headers: {
          'Content-Type': 'application/json',
        }
      })
        .then((response) => {
          if (response.ok) {
            this.isLoaded =  false;
          } else {
            throw '卸载失败';
          }
        })
        .catch((error) => {
          alert(`错误：${error}`);
          console.error('Error:', error);
        });
      this.isUnloading = false;
    }
  }
}
</script>
