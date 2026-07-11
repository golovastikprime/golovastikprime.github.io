const input = document.querySelector('#search-input');
const cards = [...document.querySelectorAll('.note-card')];
const filters = [...document.querySelectorAll('.filter')];
const count = document.querySelector('#result-count');
const empty = document.querySelector('#empty-state');
let semester = 'all';

const normalize = (value) => value.toLocaleLowerCase('ru').replaceAll('ё', 'е').trim();

function updateLibrary() {
  const words = normalize(input.value).split(/\s+/).filter(Boolean);
  let visible = 0;

  cards.forEach((card) => {
    const haystack = normalize(`${card.dataset.search} ${card.textContent}`);
    const matchesQuery = words.every((word) => haystack.includes(word));
    const matchesSemester = semester === 'all' || card.dataset.semester === semester;
    card.hidden = !(matchesQuery && matchesSemester);
    if (!card.hidden) visible += 1;
  });

  const suffix = visible % 10 === 1 && visible % 100 !== 11 ? 'материал' :
    [2, 3, 4].includes(visible % 10) && ![12, 13, 14].includes(visible % 100) ? 'материала' : 'материалов';
  count.textContent = `${visible} ${suffix}`;
  empty.hidden = visible !== 0;
}

input.addEventListener('input', updateLibrary);
document.querySelector('#search-form').addEventListener('submit', (event) => event.preventDefault());

filters.forEach((button) => button.addEventListener('click', () => {
  semester = button.dataset.filter;
  filters.forEach((filter) => {
    const active = filter === button;
    filter.classList.toggle('active', active);
    filter.setAttribute('aria-pressed', active);
  });
  updateLibrary();
}));

document.querySelectorAll('[data-query]').forEach((button) => button.addEventListener('click', () => {
  input.value = button.dataset.query;
  updateLibrary();
  document.querySelector('#library').scrollIntoView({ behavior: 'smooth' });
}));

document.querySelector('#reset-search').addEventListener('click', () => {
  input.value = '';
  filters[0].click();
  input.focus();
});

document.addEventListener('keydown', (event) => {
  if ((event.metaKey || event.ctrlKey) && event.key.toLowerCase() === 'k') {
    event.preventDefault();
    input.focus();
  }
  if (event.key === 'Escape' && document.activeElement === input) {
    input.value = '';
    updateLibrary();
    input.blur();
  }
});
