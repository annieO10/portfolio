

function showFile(filePath, buttonElement) {
  fetch(filePath)
    .then(res => res.text())
    .then(data => {
      document.getElementById("code").textContent = data;
    });

  document.querySelectorAll(".file-btn").forEach(btn => {
    btn.classList.remove("active");
  });

  buttonElement.classList.add("active");
}