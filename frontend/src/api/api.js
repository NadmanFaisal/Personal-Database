const BASE_URL = 'localhost:3001'

export async function fetchPostRequest() {
    try {
        const response = await fetch(BASE_URL + '/POST')
        const json = response.json()
        return json
    } catch(error) {
        throw new Error(error.message)
    }
}