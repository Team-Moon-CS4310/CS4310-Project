<template>
  <div id="app">
    <FileInput/>
    <FileDisplay/>
  </div>
</template>

<script>
import FileDisplay from './components/FileDisplay.vue'
import FileInput from './components/FileInput.vue'

export default {
  name: 'App',
  components: {
    FileDisplay,
    FileInput
  }
}
</script>

<style>
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  margin-top: 60px;
}
</style>
