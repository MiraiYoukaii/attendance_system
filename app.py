from flask import Flask, render_template, request, redirect, url_for, jsonify
from flask_sqlalchemy import SQLAlchemy
from datetime import datetime
import serial

app = Flask(__name__)
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///database/attendance.db'
db = SQLAlchemy(app)

# Conexão com o Arduino
arduino = serial.Serial('COM3', 9600, timeout=1)

# Modelos de Dados
class Student(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.String(100), nullable=False)
    photo = db.Column(db.String(100), nullable=False)
    class_name = db.Column(db.String(50), nullable=False)
    shift = db.Column(db.String(10), nullable=False)
    fingerprint_id = db.Column(db.String(50), unique=True, nullable=False)

class Attendance(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    student_id = db.Column(db.Integer, db.ForeignKey('student.id'), nullable=False)
    timestamp = db.Column(db.DateTime, default=datetime.utcnow)
    status = db.Column(db.String(10))

# Rota de Cadastro
@app.route('/register', methods=['GET', 'POST'])
def register():
    if request.method == 'POST':
        name = request.form['name']
        photo = request.form['photo']
        class_name = request.form['class']
        shift = request.form['shift']
        fingerprint_id = request.form['fingerprint_id']

        new_student = Student(name=name, photo=photo, class_name=class_name, shift=shift, fingerprint_id=fingerprint_id)
        db.session.add(new_student)
        db.session.commit()
        return redirect(url_for('register'))

    return render_template('register.html')

# Rota de Presença
@app.route('/attendance', methods=['GET'])
def attendance():
    if arduino.in_waiting > 0:
        data = arduino.readline().decode('utf-8').strip()
        if "REGISTERED_ID:" in data:
            fingerprint_id = data.split(":")[1]
            student = Student.query.filter_by(fingerprint_id=fingerprint_id).first()
            if student:
                status = "On Time"  # Atualizar com lógica para atrasos
                new_attendance = Attendance(student_id=student.id, status=status)
                db.session.add(new_attendance)
                db.session.commit()

    all_students = Student.query.all()
    return render_template('attendance.html', students=all_students)

if __name__ == '__main__':
    db.create_all()
    app.run(debug=True)
