const hints = [
    "Math",
    "USA",
    "USA currency",
    "Isaac Newton",
    "Albert Einstein",
    "Quantum mechanics",
    "Python programming",
    "Machine learning algorithms",
    "Docker installation",
    "Artificial Intelligence applications",
    "Blockchain technology",
    "Renaissance art",
    "French Revolution",
    "World War II",
    "Egyptian pyramids",
    "Great Depression",
    "Theory of relativity",
    "Pythagorean theorem",
    "Thermodynamics laws",
    "Atomic model of Bohr",
    "Peter the Great reforms",
    "Catherine the Great facts",
    "Steve Jobs and Apple",
    "Beethoven compositions",
    "Shakespeare's tragedies",
    "Leo Tolstoy novels",
    "Impressionism artists",
    "Brazilian rivers",
    "Population of Russia",
    "Japanese traditions",
    "France capital",
    "Internet working principles",
    "How to build a website",
    "What is AI?",
    "Writing a book guide",
    "Blockchain explained",
    "Data analysis methods",
    "Linux basics",
    "SQL vs NoSQL databases",
    "History of computers",
    "Vikings exploration",
    "Industrial Revolution",
    "Mona Lisa painting",
    "Roman Empire decline",
    "DNA structure discovery",
    "Black holes",
    "Climate change effects",
    "Tesla electric cars",
    "Mars exploration",
    "Human anatomy",
    "Periodic table of elements",
    "Photosynthesis process",
    "Newton's laws of motion",
    "C++ tutorials",
    "JavaScript frameworks",
    "HTML and CSS basics",
    "Cybersecurity tips",
    "History of the internet",
    "Mobile app development"
];

const inputField = document.getElementById("inputField")
const searchButton = document.getElementById("searchButton");
const searchInput = document.getElementById("searchInput");
const settingsButton = document.getElementById("settingsButton")
const lightButton = document.getElementById("lightButton")
const darkButton = document.getElementById("darkButton")
const defaultButton = document.getElementById("defaultButton")
const menuButton = document.getElementById("menuButton")


const changeTheme = (isDark) => {
    if (isDark) {
        document.documentElement.style.setProperty('--primary-bg-color', '#131A20');
        document.documentElement.style.setProperty('--open-windows-color', '#252B32');
        document.documentElement.style.setProperty('--text-color', '#A8AAAD');
        lightButton.style.border = null;
        darkButton.style.border = "2px solid #5C85D6";

    } else {
        document.documentElement.style.setProperty('--open-windows-color', '#ECE5DF');
        document.documentElement.style.setProperty('--primary-bg-color', '#DAD4CD');
        document.documentElement.style.setProperty('--text-color', '#1F1E1D');
        darkButton.style.border = null;
        lightButton.style.border = "2px solid #5C85D6";
    }
}

changeTheme(false)


const getHint = () => {
    return `Find an Article for example ${hints[Math.floor(Math.random() * hints.length)]}`;
}


searchButton.addEventListener('click', () => {

    searchInput.classList.toggle("hide")
    searchInput.placeholder = getHint()
});


const settings = document.getElementById("settings")

settingsButton.addEventListener('click', function() {
    settings.classList.toggle('hide');
});


lightButton.addEventListener('click', () =>{
    changeTheme(false)
})

darkButton.addEventListener('click', () => {
    changeTheme(true)
})

defaultButton.addEventListener('click', () => {
    changeTheme(false)
})



const menu = document.getElementById("menu")


menuButton.addEventListener('click', () =>{
    menu.classList.toggle("hide")
})







