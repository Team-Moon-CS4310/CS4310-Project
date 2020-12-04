import axios from 'axios'

class FileService {
  getFiles () {
    return axios.get('/getAllFiles')
  }
}

export default new FileService()
