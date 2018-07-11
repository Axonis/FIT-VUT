<?php

use Illuminate\Support\Facades\Schema;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Database\Migrations\Migration;

class CreatePatchesTable extends Migration
{
    /**
     * Run the migrations.
     *
     * @return void
     */
    public function up()
    {
        Schema::create('patches', function (Blueprint $table) {
            $table->increments('id_patch');
            $table->boolean('schvalenie');
            $table->date('datum_vydania');
            $table->date('datum_zavedenia')->nullable();
            $table->string('name_vydany');
            $table->foreign('name_vydany')->references('name')->on('users');
            $table->string('name_schvaleny')->nullable();
            $table->foreign('name_schvaleny')->references('name')->on('users');
            $table->text('diff')->nullable();
        });
    }

    /**
     * Reverse the migrations.
     *
     * @return void
     */
    public function down()
    {
        Schema::dropIfExists('patches');
    }
}
