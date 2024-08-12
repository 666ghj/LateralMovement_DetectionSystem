import 'bootstrap/dist/css/bootstrap.min.css'
import 'bootstrap/dist/js/bootstrap.bundle.min.js'
import 'bootstrap-icons/font/bootstrap-icons.css'
import { createApp } from 'vue'
import { createMemoryHistory, createRouter } from 'vue-router'
import CryptSettings from "./components/CryptSettings.vue"
import BackupSettings from "./components/BackupSettings.vue"
import FileBrowser from "./components/FileBrowser.vue"
import App from './App.vue'

function updateTheme() {
    const theme = window.matchMedia('(prefers-color-scheme: dark)').matches ? 'dark' : 'light';
    document.documentElement.setAttribute('data-bs-theme', theme);
}

window.matchMedia('(prefers-color-scheme: dark)').addEventListener('change', updateTheme);
updateTheme();

const routes = [
    { path: '/', component: CryptSettings },
    { path: '/backup', component: BackupSettings },
    { path: '/files', component: FileBrowser },
]

const router = createRouter({
    history: createMemoryHistory(),
    routes,
})

createApp(App)
    .use(router)
    .mount('#app')
