<template>
  <nav>
    <ol class="breadcrumb">
      <li class="breadcrumb-item"><a href="#" @click="gotoDir(-1)"><i class="bi bi-hdd-network"></i></a></li>
      <li class="breadcrumb-item"
        v-for="(item, index) in dir"
        :class="{ 'active': index+1 === dir.length && !isLoading }" 
        :key="index">
        <a v-if="index+1 !== dir.length" href="#" @click="gotoDir(index)">{{ item }}</a>
        <span v-else>{{ item }}</span>
      </li>
    </ol>
  </nav>

  <div class="list-group">
    <button type="button" class="list-group-item list-group-item-action" :class="{ 'disabled': isLoading }" @click="dir.pop(); list();">
      <span class="bi bi-folder-fill text-primary"></span> ..
    </button>
    <button v-for="(item, index) in files"
      type="button" class="list-group-item list-group-item-action" :key="index"
      @click="item.isFile ? getFile(item.name) : enterDir(item.name)"
      :class="{ 'disabled': isLoading }">
      <span v-if="item.isFile" class="bi bi-file-earmark-text text-primary"></span>
      <span v-else class="bi bi-folder-fill text-primary"></span>
      {{ item.name }}
    </button>
  </div>
</template>

<script>
export default {
  name: 'FileBrowser',
  data() {
    return {
      dir: [],
      files: [
        {
          name: 'folder1',
          isFile: false
        },
        {
          name: 'folder2',
          isFile: false
        },
        {
          name: 'file1.txt',
          isFile: true
        },
        {
          name: 'file2.jpg',
          isFile: true
        },
        {
          name: 'file3.mp3',
          isFile: true
        },
        {
          name: 'file4.mkv',
          isFile: true
        }
      ],
      isLoading: false,
      server: '',
    }
  },
  mounted() {
    this.init()
  },
  methods: {
    list() {
      this.isLoading = true;
      fetch('/api/list', {
        method: 'PUT',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({
          path: this.dir.join('/'),
        })
      })
        .then((response) => {
            return response.json();
        })
        .then((data) => {
          this.files = data;
        })
        .catch((error) => {
          alert(`错误：${error}`);
          console.error('Error:', error);
        });
      this.isLoading = false;
    },
    init() {
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
      this.list()
    },
    async gotoDir(index) {
      if (index < 0) {
        this.dir = [];
      }
      this.dir = this.dir.slice(0, index + 1);
      this.list();
    },
    async enterDir(name) {
      this.dir.push(name);
      this.list();
    },
    async getFile(name) {
      const link = document.createElement('a');
      link.href = `${this.server}/get/${this.dir.concat(name).join('/')}`
      link.download = name;
      link.target = "_blank";
      link.click();
    }
  }
}
</script>
