<template>
  <nav class="navbar navbar-expand-sm bg-body-tertiary">
    <div class="container">
      <a class="navbar-brand" href="/"><img src="./assets/logo.png" height="22pt"> 文件系统加密驱动</a>
    </div>
  </nav>

  <div class="container">
    <div class="row mt-4">
      <div class="d-flex align-items-start">
        <div class="nav flex-column nav-pills me-3 fw-semibold" role="tablist" aria-orientation="vertical">
          <button class="nav-link mb-4 active" data-bs-toggle="pill" data-bs-target="#home" type="button" role="tab">
            <i class="bi bi-shield-lock-fill fs-1"></i><br>加密驱动配置
          </button>
          <button class="nav-link mb-4" data-bs-toggle="pill" data-bs-target="#backup" type="button" role="tab">
            <i class="bi bi-cloud-arrow-up-fill fs-1"></i><br>远程备份设置
          </button>
          <button class="nav-link mb-4" data-bs-toggle="pill" data-bs-target="#browser" type="button" role="tab">
            <i class="bi bi-archive-fill fs-1"></i><br>查看备份文件
          </button>
        </div>
        <div class="container">
          <div class="row justify-content-around">
            <div class="col-12 col-lg-6 col-md-8">
              <div class="tab-content">
                <div class="tab-pane fade show active" id="home" role="tabpanel" tabindex="0">
                  <CryptSettings></CryptSettings>
                </div>
                <div class="tab-pane fade" id="backup" role="tabpanel" aria-labelledby="v-pills-profile-tab" tabindex="0">
                  <BackupSettings></BackupSettings>
                </div>
                <div class="tab-pane fade" id="browser" role="tabpanel" tabindex="0">
                  <FileBrowser></FileBrowser>
                </div>
              </div>
            </div>
          </div>
          
        </div>
      </div>
    </div>
  </div>

  <FixedSidebar />

  <!-- <div class="container mt-2 mb-3 text-muted small d-flex justify-content-end">
    作者：夏锦熠、何冠立、黄凯博、张原赫、张宇轩
  </div> -->
</template>

<script>
import CryptSettings from "./components/CryptSettings.vue"
import BackupSettings from "./components/BackupSettings.vue"
import FileBrowser from "./components/FileBrowser.vue"
import FixedSidebar from './components/FixedSidebar.vue';

export default {
  name: 'FileCrypt',
  components: {
    CryptSettings,
    BackupSettings,
    FileBrowser,
    FixedSidebar
  },
  mounted() {
    document.title = "文件系统加密驱动";
  }
}
</script>

<style scoped>
.nav-link {
  white-space: nowrap;
  min-width: fit-content;
}
</style>
