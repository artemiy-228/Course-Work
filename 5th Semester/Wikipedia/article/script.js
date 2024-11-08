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

const changeTheme = (isDark) => {
    if (isDark) {
        document.documentElement.style.setProperty('--primary-bg-color', '#131A20');
        document.documentElement.style.setProperty('--open-windows-color', '#252B32');
        document.documentElement.style.setProperty('--input-text-color', '#A8AAAD');
    } else {
        document.documentElement.style.setProperty('--open-windows-color', '#ECE5DF');
        document.documentElement.style.setProperty('--primary-bg-color', '#DAD4CD');
        document.documentElement.style.setProperty('--input-text-color', '#1F1E1D');
    }
}






const getHint = () => {
    return `Find an Article for example ${hints[Math.floor(Math.random() * hints.length)]}`;
}

const inputField = document.getElementById("inputField")
const searchButton = document.getElementById("searchButton");
const searchInput = document.getElementById("searchInput");
const settingsButton = document.getElementById("settingsButton")
let isOpen = false;
let isDark = false;

changeTheme(isDark)

searchButton.addEventListener('click', () => {

    if(isOpen){
        searchInput.classList.add("hide")
    }
    else{
        searchInput.classList.remove("hide")
        searchInput.placeholder = getHint()
    }
    isOpen = !isOpen;
});

settingsButton.addEventListener('click', () =>{
    isDark = !isDark
    changeTheme(isDark)
})




