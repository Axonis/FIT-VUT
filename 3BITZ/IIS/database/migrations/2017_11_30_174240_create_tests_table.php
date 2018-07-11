<?php

use Illuminate\Support\Facades\Schema;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Database\Migrations\Migration;

class CreateTestsTable extends Migration
{
    /**
     * Run the migrations.
     *
     * @return void
     */
    public function up()
    {
        Schema::create('tests', function (Blueprint $table) {
            $table->increments('id');
            $table->date('datum_zaciatku')->nullable();
            $table->date('datum_konca')->nullable();
            $table->float('hodnotenie')->nullable();
            $table->unsignedInteger('id_patch');
            $table->string('napisal')->nulable();
            $table->string('name_schvaleny')->nullable();
            $table->text('code');

            $table->foreign('id_patch')->references('id_patch')->on('patches');
            $table->foreign('name_schvaleny')->references('name')->on('users');

        });
    }

    /**
     * Reverse the migrations.
     *
     * @return void
     */
    public function down()
    {
        Schema::dropIfExists('tests');
    }
}
