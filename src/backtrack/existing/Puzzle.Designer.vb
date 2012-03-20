<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class Puzzle
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    <System.Diagnostics.DebuggerNonUserCode()> _
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Me.ButtonGenerate = New System.Windows.Forms.Button
        Me.PanelLoad = New System.Windows.Forms.Panel
        Me.LabelLoad = New System.Windows.Forms.Label
        Me.PanelLoad.SuspendLayout()
        Me.SuspendLayout()
        '
        'ButtonGenerate
        '
        Me.ButtonGenerate.Location = New System.Drawing.Point(12, 492)
        Me.ButtonGenerate.Name = "ButtonGenerate"
        Me.ButtonGenerate.Size = New System.Drawing.Size(75, 23)
        Me.ButtonGenerate.TabIndex = 0
        Me.ButtonGenerate.Text = "Generate"
        Me.ButtonGenerate.UseVisualStyleBackColor = True
        '
        'PanelLoad
        '
        Me.PanelLoad.BackColor = System.Drawing.Color.WhiteSmoke
        Me.PanelLoad.Controls.Add(Me.LabelLoad)
        Me.PanelLoad.Location = New System.Drawing.Point(12, 12)
        Me.PanelLoad.Name = "PanelLoad"
        Me.PanelLoad.Size = New System.Drawing.Size(470, 470)
        Me.PanelLoad.TabIndex = 1
        Me.PanelLoad.Visible = False
        '
        'LabelLoad
        '
        Me.LabelLoad.AutoSize = True
        Me.LabelLoad.BackColor = System.Drawing.Color.Transparent
        Me.LabelLoad.Font = New System.Drawing.Font("Boopee", 40.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LabelLoad.ForeColor = System.Drawing.Color.SteelBlue
        Me.LabelLoad.Location = New System.Drawing.Point(146, 202)
        Me.LabelLoad.Name = "LabelLoad"
        Me.LabelLoad.Size = New System.Drawing.Size(179, 67)
        Me.LabelLoad.TabIndex = 1
        Me.LabelLoad.Text = "Loading..."
        '
        'Puzzle
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(494, 604)
        Me.Controls.Add(Me.PanelLoad)
        Me.Controls.Add(Me.ButtonGenerate)
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle
        Me.Name = "Puzzle"
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
        Me.Text = "Puzzle"
        Me.PanelLoad.ResumeLayout(False)
        Me.PanelLoad.PerformLayout()
        Me.ResumeLayout(False)

    End Sub
    Friend WithEvents ButtonGenerate As System.Windows.Forms.Button
    Friend WithEvents PanelLoad As System.Windows.Forms.Panel
    Friend WithEvents LabelLoad As System.Windows.Forms.Label
End Class
